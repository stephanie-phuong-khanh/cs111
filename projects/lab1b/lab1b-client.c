// NAME: Stephanie Doan
// EMAIL: stephaniekdoan@ucla.edu
// ID: 604981556

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <netdb.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <ulimit.h>
#include <unistd.h>
#include <zlib.h>

#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define CR 0x0D
#define LF 0x0A
#define CTRL_C 0x03
#define CTRL_D 0x04

struct termios termios_orig;
static char cr_lf[2] = {CR, LF};
static const int BUF_SIZE = 512;

void print_error(char *error_string, int errnum)
{
    fprintf(stderr, "%s: %s\n", error_string, strerror(errnum));
    exit(1);
}

static void restore_terminal()
{
    if (-1 == tcsetattr(/*fd=*/0, /*optional_actions=*/TCSANOW, /*termios_p=*/&termios_orig))
        print_error("Failed to restore original terminal settings", errno);
}

void init_terminal()
{
    if (-1 == tcgetattr(/*fd=*/0, /*termios_p=*/&termios_orig))
        print_error("Failed to save original terminal settings", errno);

    struct termios termios_new = termios_orig;
    termios_new.c_iflag = ISTRIP; /* only lower 7 bits */
    termios_new.c_oflag = 0;      /* no processing */
    termios_new.c_lflag = 0;      /* no processing */
    if (-1 == tcsetattr(/*fd=*/0, /*optional_actions=*/TCSANOW, /*termios_p=*/&termios_new))
        print_error("Failed to set new terminal settings", errno);
}

void write_to_log(int log_fd, char *type, char *buffer, int bytes)
{
    if (-1 == log_fd)
        return;
    dprintf(log_fd, "%s %d bytes: ", type, bytes);
    write(log_fd, buffer, bytes);
    write(log_fd, "\n", 1);
}

void run_client(int socket_fd, int log_fd, int compress)
{
    static const int NFDS = 2, FDS_KEY_IDX = 0, FDS_SOCKET_IDX = 1;
    struct pollfd fds[NFDS];
    fds[FDS_KEY_IDX].fd = STDIN_FILENO; /* Reads input from keyboard */
    fds[FDS_KEY_IDX].events = POLLIN;
    fds[FDS_SOCKET_IDX].fd = socket_fd; /* Reads input from socket */
    fds[FDS_SOCKET_IDX].events = POLLIN;

    z_stream key_stream, socket_stream; /* Used only if compress is set */
    if (compress)
    {
        key_stream.zalloc = Z_NULL;
        key_stream.zfree = Z_NULL;
        key_stream.opaque = Z_NULL;

        socket_stream.zalloc = Z_NULL;
        socket_stream.zfree = Z_NULL;
        socket_stream.opaque = Z_NULL;
    }

    char buf[BUF_SIZE], cbuf[BUF_SIZE];
    int poll_ret = 0, bytes_read = 0;
    while ((poll_ret = poll(fds, NFDS, /*timeout=*/0)) != -1)
    {
        if (fds[FDS_KEY_IDX].revents & POLLIN) /* From keyboard */
        {
            if ((bytes_read = read(STDIN_FILENO, &buf, BUF_SIZE)) == -1)
                print_error("Failed to read input from terminal", errno);

            // Echo keyboard input to terminal for readability
            int i;
            for (i = 0; i < bytes_read; ++i)
            {
                if (buf[i] == CR || buf[i] == LF)
                    write(STDOUT_FILENO, cr_lf, 2);
                else
                    write(STDOUT_FILENO, &buf[i], 1);
            }

            if (compress)
            {
                if (deflateInit(&key_stream, Z_DEFAULT_COMPRESSION) != Z_OK)
                {
                    fprintf(stderr, "Failed to initialize compression on client\n");
                    exit(1);
                }
                key_stream.next_in = (unsigned char *)buf;
                key_stream.avail_in = bytes_read;
                key_stream.next_out = (unsigned char *)cbuf;
                key_stream.avail_out = BUF_SIZE;
                while (key_stream.avail_in > 0)
                {
                    if (deflate(&key_stream, Z_SYNC_FLUSH) != Z_OK)
                    {
                        fprintf(stderr, "Failed to compress data\n");
                        exit(1);
                    }
                };

                int bytes = BUF_SIZE - key_stream.avail_out;
                write(socket_fd, cbuf, bytes);
                write_to_log(log_fd, "SENT", cbuf, bytes);
                deflateEnd(&key_stream);
            }
            else
            {
                write(socket_fd, buf, bytes_read);
                write_to_log(log_fd, "SENT", buf, bytes_read);
            }
        }

        if (fds[FDS_SOCKET_IDX].revents & POLLIN) /* From socket */
        {
            if ((bytes_read = read(socket_fd, &buf, BUF_SIZE)) == -1)
                print_error("Failed to read output from shell", errno);
            if (bytes_read == 0)
                exit(0);

            if (compress)
            {
                if (inflateInit(&socket_stream) != Z_OK)
                {
                    fprintf(stderr, "Failed to initialize decompression on client\n");
                    exit(1);
                }
                socket_stream.next_in = (unsigned char *)buf;
                socket_stream.avail_in = bytes_read;
                socket_stream.next_out = (unsigned char *)cbuf;
                socket_stream.avail_out = BUF_SIZE;
                while (socket_stream.avail_in > 0)
                {
                    if (inflate(&socket_stream, Z_SYNC_FLUSH) != Z_OK)
                    {
                        fprintf(stderr, "Failed to decompress data\n");
                        exit(1);
                    }
                }

                write(STDOUT_FILENO, cbuf, BUF_SIZE - socket_stream.avail_out);
                deflateEnd(&socket_stream);
            }
            else
            {
                write(STDOUT_FILENO, buf, bytes_read);
            }

            write_to_log(log_fd, "RECEIVED", buf, bytes_read);
        }

        if ((fds[FDS_KEY_IDX].revents & (POLLHUP | POLLERR)) || (fds[FDS_SOCKET_IDX].revents & (POLLHUP | POLLERR)))
        {
            fprintf(stderr, "Ended server connection\n");
            exit(0);
        }
    }
    if (-1 == poll_ret)
        print_error("Failure on poll", errno);
}

int connect_to_server(char host[], int port)
{
    struct hostent *h = gethostbyname(host);
    if (h == NULL)
        print_error("Failed get host by name", errno);

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == socket_fd)
        print_error("Failed to open socket on client", errno);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    memcpy(&(server_address.sin_addr.s_addr), h->h_addr, h->h_length);
    if (-1 == connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)))
        print_error("Failed to connect client to server", errno);

    return socket_fd;
}

int main(int argc, char *argv[])
{
    static struct option long_options[] = {
        {"port", required_argument, 0, 'p'},
        {"log", required_argument, 0, 'l'},
        {"host", required_argument, 0, 'h'},
        {"compress", no_argument, 0, 'c'},
        {0, 0, 0, 0}};

    int ch = 0, option_index = 0;
    int port = 0, log = 0, compress = 0; /* Flags */
    char *filename = "", *hostname = "localhost";
    int port_number = -1, log_fd = -1;

    while ((ch = getopt_long(argc, argv, "", long_options, &option_index)) != -1)
    {
        switch (ch)
        {
        case 'p':
            port = 1;
            port_number = atoi(optarg);
            break;
        case 'l':
            log = 1;
            filename = optarg;
            break;
        case 'h':
            hostname = optarg;
            break;
        case 'c':
            compress = 1;
            break;
        default:
            fprintf(stderr, "Invalid argument\n");
            exit(1);
        }
    }

    // Set up according to flags
    if (log)
    {
        struct rlimit limit;
        limit.rlim_cur = 10000; /* Soft limit */
        limit.rlim_max = 10000; /* Hard limit */
        if (-1 == setrlimit(/*resource=*/RLIMIT_FSIZE, &limit))
            print_error("Failed to set resource limit", errno);

        log_fd = creat(filename, /*mode=*/0666);
        if (-1 == log_fd)
            print_error("Failed to open logfile", errno);
    }
    if (!port)
    {
        fprintf(stderr, "Must provide --port option.\n");
        exit(1);
    }

    // Terminal behavior
    init_terminal();
    if (-1 == atexit(restore_terminal))
        print_error("Failed to register terminal restore function to be called at process termination", errno);

    // Connect and run client
    int socket_fd = connect_to_server(hostname, port_number);
    run_client(socket_fd, log_fd, compress);

    return 0;
}
