// NAME: Ashley Zhu, Stephanie Doan
// EMAIL: ashleytz@g.ucla.edu, stephaniekdoan@ucla.edu
// ID: 505308582 604981556

#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/stat.h>

#include "ext2_fs.h"

#define EXT2_SUPER_MAGIC 0xEF53
#define DOUBLE_OFFSET 256
#define TRIPLE_OFFSET 65536

int fd;

void print_error(std::string error_string, int errnum, int exit_code)
{
    if (errnum == -1)
        std::cerr << error_string << std::endl;
    else
        std::cerr << error_string << ": " << std::strerror(errnum) << std::endl;
    exit(exit_code);
}

std::string time_format(unsigned int seconds)
{
    std::stringstream ss;
    time_t time = static_cast<time_t>(seconds);
    struct tm *ptm = gmtime(&time);
    ss << std::setfill('0') << std::setw(2) << ptm->tm_mon + 1
       << '/' << std::setfill('0') << std::setw(2) << ptm->tm_mday
       << '/' << std::setfill('0') << std::setw(2) << (ptm->tm_year + 1900) % 100
       << ' ' << std::setfill('0') << std::setw(2) << ptm->tm_hour
       << ':' << std::setfill('0') << std::setw(2) << ptm->tm_min
       << ':' << std::setfill('0') << std::setw(2) << ptm->tm_sec;
    return ss.str();
}

void indirect(unsigned int inode_index, __uint32_t block_num, int level, unsigned int offset, unsigned int bsize)
{
    __uint32_t blocks[bsize / sizeof(__uint32_t)];
    static const int kLogicalBlockOffset = 1024;
    if (-1 == pread(fd, &blocks, sizeof(blocks), /*offset=*/bsize * (block_num - 1) + kLogicalBlockOffset))
        print_error("Failed to read file", errno, 1);

    for (unsigned int block_idx = 0; block_idx < bsize / sizeof(__uint32_t); ++block_idx)
    {
        if (!blocks[block_idx])
            continue;
        std::cout << "INDIRECT," << /*i-node number of the owning file*/ inode_index + 1
                  << "," << /*level of indirection for the block being scanned*/ level
                  << "," << /*logical block offset represented by the referenced block*/ offset + block_idx
                  << "," << /*block number of indirect block being scanned*/ block_num
                  << "," << /*block number of referenced block*/ blocks[block_idx] << std::endl;
        if (level == 1)
            continue;
        indirect(inode_index, blocks[block_idx], level - 1, offset, bsize);
        if (level == 2)
            offset += DOUBLE_OFFSET;
        else if (level == 3)
            offset += TRIPLE_OFFSET;
    }
}

void inode_summary(unsigned int inode_index, unsigned int inode_table, unsigned int bsize)
{
    struct ext2_inode inode;
    if (-1 == pread(fd, &inode, sizeof(ext2_inode), /*offset=*/(inode_table * bsize) + (inode_index * sizeof(ext2_inode))))
        print_error("Failed to read file", errno, 1);
    if (!(inode.i_mode && inode.i_links_count))
        return;

    char file_type = '?';
    if (S_ISREG(inode.i_mode))
        file_type = 'f';
    else if (S_ISDIR(inode.i_mode))
        file_type = 'd';
    else if (S_ISLNK(inode.i_mode))
        file_type = 's';

    std::cout << "INODE," << /*inode number*/ inode_index + 1
              << "," << /*file type*/ file_type
              << "," << std::oct << /*mode*/ (inode.i_mode & 0x0FFF) << std::dec
              << "," << /*owner*/ inode.i_uid
              << "," << /*group*/ inode.i_gid
              << "," << /*link count*/ inode.i_links_count
              << "," << /*time of last I-node change*/ time_format(inode.i_ctime)
              << "," << /*modification time*/ time_format(inode.i_mtime)
              << "," << /*time of last access*/ time_format(inode.i_atime)
              << "," << /*file size*/ inode.i_size
              << "," << /*number of blocks of disk space taken up by this file*/ inode.i_blocks;
    if ((file_type == 's' && inode.i_size > 60) || file_type == 'f' || file_type == 'd')
    {
        for (int i = 0; i < EXT2_N_BLOCKS; ++i) /*Print 15 block numbers*/
            std::cout << "," << inode.i_block[i];
    }
    std::cout << std::endl;

    // Direct blocks
    if (file_type == 'd')
    {
        for (int i = 0; i < EXT2_NDIR_BLOCKS; ++i)
        {
            if (!inode.i_block[i])
                continue;
            struct ext2_dir_entry dir_entry;
            for (unsigned int offset = 0; offset < bsize; offset += dir_entry.rec_len)
            {
                if (-1 == pread(fd, &dir_entry, sizeof(ext2_dir_entry), /*offset=*/inode.i_block[i] * bsize + offset))
                    print_error("Failed to read file", errno, 1);
                if (dir_entry.inode)
                {
                    std::cout << "DIRENT," << /*parent inode number*/ inode_index + 1
                              << "," << /*logical byte offset*/ offset
                              << "," << /*inode number of the referenced file*/ dir_entry.inode
                              << "," << /*entry length*/ dir_entry.rec_len
                              << "," << /*name length*/ static_cast<int>(dir_entry.name_len)
                              << ",'" << /*name*/ dir_entry.name << "'" << std::endl;
                }
            }
        }
    }

    // Indirect blocks
    if (inode.i_block[EXT2_IND_BLOCK]) /*indirect block = 13th block*/
        indirect(inode_index, /*block_num=*/inode.i_block[EXT2_IND_BLOCK], /*level=*/1, /*offset=*/EXT2_NDIR_BLOCKS, bsize);
    if (inode.i_block[EXT2_DIND_BLOCK]) /*doubly indirect block = 14th block*/
        indirect(inode_index, /*block_num=*/inode.i_block[EXT2_DIND_BLOCK], /*level=*/2, /*offset=*/EXT2_NDIR_BLOCKS + DOUBLE_OFFSET, bsize);
    if (inode.i_block[EXT2_TIND_BLOCK]) /*triply indirect block = 15th block*/
        indirect(inode_index, /*block_num=*/inode.i_block[EXT2_TIND_BLOCK], /*level=*/3, /*offset=*/EXT2_NDIR_BLOCKS + DOUBLE_OFFSET + TRIPLE_OFFSET, bsize);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        print_error("Incorrect number of arguments to program", -1, 1);

    fd = open(/*pathname=*/argv[1], O_RDONLY);
    if (fd == -1)
        print_error("Failed to open file", errno, 1);

    // Superblock
    struct ext2_super_block superblock;
    static const int kLogicalBlockOffset = 1024;
    if (-1 == pread(fd, &superblock, sizeof(ext2_super_block), /*offset=*/kLogicalBlockOffset))
        print_error("Failed to read file", errno, 1);
    if (superblock.s_magic != EXT2_SUPER_MAGIC)
        print_error("Superblock was not read", -1, 1);
    unsigned int bsize = EXT2_MIN_BLOCK_SIZE << superblock.s_log_block_size;
    std::cout << "SUPERBLOCK," << /*total number of blocks*/ superblock.s_blocks_count
              << "," << /*total number of i-nodes*/ superblock.s_inodes_count
              << "," << /*block size*/ bsize
              << "," << /*i-node size*/ superblock.s_inode_size
              << "," << /*blocks per group*/ superblock.s_blocks_per_group
              << "," << /*i-nodes per group*/ superblock.s_inodes_per_group
              << "," << /*first non-reserved i-node*/ superblock.s_first_ino << std::endl;

    // Block group descriptor table
    int groups = (superblock.s_blocks_count / superblock.s_blocks_per_group) + 1;
    struct ext2_group_desc group;
    for (int i = 0; i < groups; ++i)
    {
        if (-1 == pread(fd, &group, sizeof(ext2_group_desc), /*offset=*/kLogicalBlockOffset + sizeof(ext2_super_block) + i * sizeof(ext2_group_desc)))
            print_error("Failed to read file", errno, 1);
        std::cout << "GROUP," << /*group number*/ i
                  << "," << /*total number of blocks in this group*/ superblock.s_blocks_count
                  << "," << /*total number of i-nodes in this group*/ superblock.s_inodes_count
                  << "," << /*number of free blocks*/ group.bg_free_blocks_count
                  << "," << /*number of free i-nodes*/ group.bg_free_inodes_count
                  << "," << /*block number of free block bitmap for this group*/ group.bg_block_bitmap
                  << "," << /*block number of free i-node bitmap for this group*/ group.bg_inode_bitmap
                  << "," << /*block number of first block of i-nodes in this group*/ group.bg_inode_table
                  << std::endl;

        // Block entries (1=used, 0=free)
        unsigned char block_bitmap[bsize];
        if (-1 == pread(fd, block_bitmap, bsize, /*offset=*/group.bg_block_bitmap * bsize))
            print_error("Failed to read file", errno, 1);
        for (unsigned int byte_idx = 0; byte_idx < bsize; ++byte_idx)
        {
            unsigned char mask = 1;
            for (int bit_idx = 0; bit_idx < 8; ++bit_idx)
            {
                unsigned int index = (8 * byte_idx) + bit_idx + 1;
                if (!(block_bitmap[byte_idx] & mask))
                    std::cout << "BFREE," << /*number of the free block*/ index << std::endl;
                mask <<= 1;
            }
        }

        // I-node entries (1=used, 0=free)
        unsigned char inode_bitmap[bsize];
        if (-1 == pread(fd, inode_bitmap, bsize, /*offset=*/group.bg_inode_bitmap * bsize))
            print_error("Failed to read file", errno, 1);
        unsigned int inodes = superblock.s_inodes_per_group / 8;
        for (unsigned int byte_idx = 0; byte_idx < inodes; ++byte_idx)
        {
            unsigned char mask = 1;
            for (int bit_idx = 0; bit_idx < 8; ++bit_idx)
            {
                unsigned int index = (8 * byte_idx) + bit_idx + 1;
                if (!(inode_bitmap[byte_idx] & mask))
                    std::cout << "IFREE," << /*number of the free i-node*/ index << std::endl;
                else
                    inode_summary(index, group.bg_inode_table, bsize);
                mask <<= 1;
            }
        }
    }

    return 0;
}
