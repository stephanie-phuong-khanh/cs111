# NAME: Ashley Zhu, Stephanie Doan
# EMAIL: ashleytz@g.ucla.edu, stephaniekdoan@ucla.edu
# ID: 505308582, 604981556

#!/usr/bin/python

import sys
import csv
from collections import defaultdict

SINGLE_OFFSET = 12
DOUBLE_OFFSET = 256
TRIPLE_OFFSET = 65536
DOUBLE_INDIRECT = SINGLE_OFFSET + DOUBLE_OFFSET
TRIPLE_INDIRECT = DOUBLE_INDIRECT + TRIPLE_OFFSET

exit_status = 0
superblock = None

dirents = []

free_inodes = set() # inode number
allocated_inodes = {} # inode number to inode
free_blocks = set() # block number
allocated_blocks = defaultdict(list) #  block number to list of blocks

def error(msg):
    '''
    Prints error message and exits
    '''
    sys.stderr.write(msg)
    sys.exit(1)

class SuperBlock:
    '''
    SUPERBLOCK

    total number of blocks (decimal)
    total number of i-nodes (decimal)
    block size (in bytes, decimal)
    i-node size (in bytes, decimal)
    blocks per group (decimal)
    i-nodes per group (decimal)
    first non-reserved i-node (decimal)
    '''
    def __init__(self, block_count, inode_count, first_nonreserved_inode):
        self.block_count = block_count
        self.inode_count = inode_count
        self.first_nonreserved_inode = first_nonreserved_inode


class Block:
    def __init__(self, inode, offset):
        self.inode = inode
        self.offset = offset

class Inode:
    '''
    INODE

    inode number (decimal)
    file type ('f' for file, 'd' for directory, 's' for symbolic link, '?" for anything else)
    mode (low order 12-bits, octal ... suggested format "%o")
    owner (decimal)
    group (decimal)
    link count (decimal)
    time of last I-node change (mm/dd/yy hh:mm:ss, GMT)
    modification time (mm/dd/yy hh:mm:ss, GMT)
    time of last access (mm/dd/yy hh:mm:ss, GMT)
    file size (decimal)
    number of (512 byte) blocks of disk space (decimal) taken up by this file
    '''
    def __init__(self, inode, link_count):
        self.inode = inode
        self.link_count = link_count

class Dirent:
    '''
    DIRENT

    parent inode number (decimal) ... the I-node number of the directory that contains this entry
    logical byte offset (decimal) of this entry within the directory
    inode number of the referenced file (decimal)
    entry length (decimal)
    name length (decimal)
    name (string, surrounded by single-quotes). Don't worry about escaping, we promise there will be no single-quotes or commas in any of the file names.
    '''
    def __init__(self, parent, inode, name):
        self.parent = parent
        self.inode = inode
        self.name = name

def print_corruption(msg):
    '''
    Prints non-block-related errors and switch exit status
    '''
    print(msg)
    exit_status = 2

def print_inconsistent_block(error, block_num, inode, offset):
    '''
    Print block-related errors and switches exit status
    '''
    indirection = ""
    if offset >= TRIPLE_INDIRECT:
        indirection = "TRIPLE INDIRECT "    
    elif offset >= DOUBLE_INDIRECT:
        indirection = "DOUBLE INDIRECT "
    elif offset >= SINGLE_OFFSET:
        indirection = "INDIRECT "
    print "{} {}BLOCK {} IN INODE {} AT OFFSET {}".format(error, indirection, block_num, inode, offset)
    exit_status = 2

def scan_indirect(props):
    '''
    Scans INDIRECT

    I-node number of the owning file (decimal)
    (decimal) level of indirection for the block being scanned ... 1 for single indirect, 2 for double indirect, 3 for triple
    logical block offset (decimal) represented by the referenced block. If the referenced block is a data block, this is the logical block offset of that block within the file. If the referenced block is a single- or double-indirect block, this is the same as the logical offset of the first data block to which it refers.
    block number of the (1, 2, 3) indirect block being scanned (decimal)... not the highest level block (in the recursive scan), but the lower level block that contains the block reference reported by this entry.
    block number of the referenced block (decimal)
    '''
    inode = int(props[1])
    block_num = int(props[5])
    logical_offset = props[3]
    if block_num:
        if block_num < 0 or superblock.block_count <= block_num:
            print_inconsistent_block("INVALID", block_num, inode, logical_offset)
        elif block_num < superblock.first_nonreserved_inode:
            print_inconsistent_block("RESERVED", block_num, inode, logical_offset)
        else:
            allocated_blocks[block_num].append(Block(inode, logical_offset))

def scan_inode(props):
    '''
    Scans inode
    '''
    #links with a short length (60 bytes or less): the i-node block pointer fields do not contain block numbers, should not be analyzed
    if props[2] == 's' and int(props[10]) <= 60:
        return

    inode = int(props[1])
    logical_offset = 0
    #iterate through the blocks of the inode, located in props 12 to 26
    for idx in range(12,27):
        block_num = int(props[idx])
        if block_num:
            #check for INVALID blocks: number is less than zero or greater than the highest block in the file system
            #check for RESERVED blocks: number is less than 8 on EXT2
            #otherwise add it to existing blocks
            if block_num < 0 or superblock.block_count <= block_num:
                print_inconsistent_block("INVALID", block_num, inode, logical_offset)
            elif block_num < 8:
                print_inconsistent_block("RESERVED", block_num, inode, logical_offset)
            else:
                allocated_blocks[block_num].append(Block(inode, logical_offset))
        if idx < 24:
            logical_offset += 1
        elif idx == 24:
            logical_offset += DOUBLE_OFFSET
        elif idx == 25:
            logical_offset += TRIPLE_OFFSET

def scan_directory():
    '''
    Scans directory
    '''
    #check for inconsistent links
    string = "INODE {} HAS {} LINKS BUT LINKCOUNT IS {}"
    for inode in allocated_inodes:
        inode_details = allocated_inodes[inode]
        refs = inode_details.link_count
        for dirent in dirents:
            if dirent.inode == inode_details.inode:
                refs -= 1
        if refs != 0:
            print_corruption(string.format(inode_details.inode, inode_details.link_count - refs, inode_details.link_count))

    inode_to_parent = {}    # inode number to parent's inode number

    for dirent in dirents:
        if dirent.inode <= superblock.inode_count:
            #if the directory is not the current or a parent directory, set the inode of the current directory to its parent inode
            if dirent.inode == 2:
                inode_to_parent[dirent.inode] = dirent.inode
            elif dirent.name != "'..'" and dirent.name != "'.'":
                inode_to_parent[dirent.inode] = dirent.parent
    
        #check if inodes pointing to current and parent directories are equal to themselves
        string = "DIRECTORY INODE {} NAME {} LINK TO INODE {} SHOULD BE {}"
        if dirent.name == "'.'" and dirent.inode != dirent.parent:
            print_corruption(string.format(dirent.parent, dirent.name, dirent.inode, dirent.parent))
        if dirent.name == "'..'" and dirent.inode != inode_to_parent[dirent.parent]:
            print_corruption(string.format(dirent.parent, dirent.name, dirent.inode, dirent.parent))

        #check if the inodes' parents are correct according to directory entries
        string = "DIRECTORY INODE {} NAME {} {} INODE {}"

        cur_inode = allocated_inodes[dirent.inode] if dirent.inode in allocated_inodes else None
        if not cur_inode and 0 < dirent.inode and dirent.inode < superblock.inode_count:
            print_corruption(string.format(dirent.parent, dirent.name, "UNALLOCATED", dirent.inode))
        elif not cur_inode or cur_inode.inode < 1 or superblock.inode_count < cur_inode.inode:
            print_corruption(string.format(dirent.parent, dirent.name, "INVALID", dirent.inode))
    
def check_block_inconsistencies():
    '''
    Checks for block inconsistencies
    '''
    for block_num in range(superblock.block_count):
        if block_num not in free_blocks and block_num not in allocated_blocks and block_num >= 8:
            print_corruption("UNREFERENCED BLOCK {}".format(block_num))
        elif block_num in free_blocks and block_num in allocated_blocks:
            print_corruption("ALLOCATED BLOCK {} ON FREELIST".format(block_num))

def check_allocated_inodes_free_list():
    '''
    Checks for allocated inodes on free list
    '''
    for inode_num in allocated_inodes:
        if inode_num in free_inodes:
            print_corruption("ALLOCATED INODE {} ON FREELIST".format(inode_num))

def check_inode_inconsistencies():
    '''
    Checks for inode reference inconsistencies
    '''
    for inode in range(superblock.first_nonreserved_inode, superblock.inode_count):
        free = inode in free_inodes
        allocated = inode in allocated_inodes
        if not inode in free_inodes and not inode in allocated_inodes:
            print_corruption("UNALLOCATED INODE {} NOT ON FREELIST".format(inode))

def check_duplicate_blocks():
    '''
    Check for duplicate blocks
    '''
    for block_num, blocks in allocated_blocks.items():
        if len(blocks) > 1:
            for block in blocks:
                print_inconsistent_block("DUPLICATE", block_num, block.inode, block.offset)

def main():
    if len(sys.argv) != 2:
        error("There should be only one argument for this program.\n")

    try:
        with open(sys.argv[1], 'rb') as csvfile:
            r = csv.reader(csvfile, delimiter=',')
            for line in r:
                obj_type = line[0]
                if obj_type == "INODE":
                    scan_inode(line)                                                
                    allocated_inodes[int(line[1])] = Inode(int(line[1]), int(line[6]))  # add inode to list of used inodes (no need to check for duplicates)
                elif obj_type == "INDIRECT":
                    scan_indirect(line)                                                 # scan the block
                elif obj_type == "IFREE":
                    free_inodes.add(int(line[1]))                                       # add inode number to free inodes structure
                elif obj_type == "BFREE":
                    free_blocks.add(int(line[1]))                                       # add block to free blocks structure
                elif obj_type == "DIRENT":
                    dirents.append(Dirent(int(line[1]), int(line[3]), line[6]))         # add directory entry
                elif obj_type == "SUPERBLOCK":
                    global superblock
                    superblock = SuperBlock(int(line[1]), int(line[2]), int(line[7]))   # create superblock
    except IOError:
        error(sys.argv[1] + ' could not be opened.\n')

    scan_directory()
    check_block_inconsistencies()
    check_allocated_inodes_free_list()
    check_inode_inconsistencies()
    check_duplicate_blocks()

    sys.exit(exit_status)

if __name__ == "__main__":
    main()
