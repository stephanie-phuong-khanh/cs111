// NAME: Stephanie Doan
// EMAIL: stephaniekdoan@ucla.edu
// ID: 604981556

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <zlib.h>

#include <sys/socket.h>
#include <sys/wait.h>

#define CR 0x0D
#define LF 0x0A
#define CTRL_C 0x03
#define CTRL_D 0x04

static char cr_lf[2] = {CR, LF};
static const int BUF_SIZE = 512;

void print_error(char *error_string, int errnum)
{
    fprintf(stderr, "%s: %s\n", error_string, strerror(errnum));
    exit(1);
}

int connect_to_client(int port)
{
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == server_socket_fd)
        print_error("Failed to open socket on client", errno);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY; /* Accept any incoming message */

    if (-1 == bind(server_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)))
        print_error("Failed to bind server", errno);

    if (-1 == listen(server_socket_fd, /*backlog=*/5))
        print_error("Failed to bind server", errno);

    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);
    int client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&client_address, &client_address_length);
    if (-1 == client_socket_fd)
        print_error("Failed to accept connection on socket", errno);

    return client_socket_fd;
}

void run_server(char *program, int client_socket_fd, int compress)
{
    int pipefd_to_shell[2], pipefd_from_shell[2];
    static const int READ_END = 0, WRITE_END = 1;
    if (-1 == pipe(pipefd_to_shell) || -1 == pipe(pipefd_from_shell))
        print_error("Failed to create pipe", errno);

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

    int pid = fork();
    if (-1 == pid)
        print_error("Failed to create new process with fork", errno);
    if (0 == pid) /* Child process */
    {
        close(pipefd_to_shell[WRITE_END]); /* Close unused write end */
        dup2(/*oldfd=*/pipefd_to_shell[READ_END], /*newfd=*/STDIN_FILENO);
        close(pipefd_to_shell[READ_END]);

        close(pipefd_from_shell[READ_END]); /* Close unused read end */
        dup2(/*oldfd=*/pipefd_from_shell[WRITE_END], /*newfd=*/STDOUT_FILENO);
        close(pipefd_from_shell[WRITE_END]);

        if (-1 == execl(program, program, (char *)NULL))
            print_error("Failed to exec program with shell", errno);
    }
    else /* Parent process */
    {
        close(pipefd_to_shell[READ_END]);
        close(pipefd_from_shell[WRITE_END]);

        static const int NFDS = 2, FDS_SOCKET_IDX = 0, FDS_SHELL_IDX = 1;
        struct pollfd fds[NFDS];
        fds[FDS_SOCKET_IDX].fd = client_socket_fd; /* Reads input from socket */
        fds[FDS_SOCKET_IDX].events = POLLIN;
        fds[FDS_SHELL_IDX].fd = pipefd_from_shell[READ_END]; /* Reads input from shell */
        fds[FDS_SHELL_IDX].events = POLLIN;

        int poll_ret = 0, bytes_read = 0;
        char buf[BUF_SIZE], cbuf[BUF_SIZE];
        while ((poll_ret = poll(fds, NFDS, /*timeout=*/0)) != -1)
        {
            int status;
            int changed_child = waitpid(pid, &status, WNOHANG);
            if (-1 == changed_child) /* Wait for state changes in child process */
                print_error("Failed to await completion of child process", errno);
            else if (changed_child < 0)
            {
                fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n\r", WTERMSIG(status), WEXITSTATUS(status));
                exit(0);
            }

            if (fds[FDS_SOCKET_IDX].revents & POLLIN) /* From client */
            {
                if ((bytes_read = read(client_socket_fd, &buf, BUF_SIZE)) == -1)
                    print_error("Failed to read input from terminal", errno);

                if (compress)
                {
                    if (inflateInit(&key_stream) != Z_OK)
                    {
                        fprintf(stderr, "Failed to initialize decompression on server\n");
                        exit(1);
                    }
                    key_stream.next_in = (unsigned char *)buf;
                    key_stream.avail_in = bytes_read;
                    key_stream.next_out = (unsigned char *)cbuf;
                    key_stream.avail_out = BUF_SIZE;

                    while (key_stream.avail_in > 0)
                    {
                        if (inflate(&key_stream, Z_SYNC_FLUSH) != Z_OK)
                        {
                            fprintf(stderr, "Failed to decompress data\n");
                            exit(1);
                        }
                    }

                    int i, bytes = BUF_SIZE - key_stream.avail_out;
                    for (i = 0; i < bytes; ++i)
                    {
                        switch (cbuf[i])
                        {
                        case CR:
                        case LF:
                            write(pipefd_to_shell[WRITE_END], &cr_lf[1], 1);
                            break;
                        case CTRL_C: /* SIGINT to shell */
                            kill(pid, SIGINT);
                            break;
                        case CTRL_D: /* Close write side of pipe to shell */
                            close(pipefd_to_shell[WRITE_END]);
                            int status;
                            if (-1 == waitpid(pid, &status, 0))
                                print_error("Failed to await completion of child process", errno);
                            fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n\r", WTERMSIG(status), WEXITSTATUS(status));
                            exit(0);
                        default:
                            write(pipefd_to_shell[WRITE_END], &cbuf[i], 1);
                        }
                    }

                    inflateEnd(&key_stream);
                }
                else
                {
                    int i;
                    for (i = 0; i < bytes_read; ++i)
                    {
                        switch (buf[i])
                        {
                        case CR:
                        case LF:
                            write(pipefd_to_shell[WRITE_END], &cr_lf[1], 1);
                            break;
                        case CTRL_C: /* SIGINT to shell */
                            kill(pid, SIGINT);
                            break;
                        case CTRL_D: /* Close write side of pipe to shell */
                            close(pipefd_to_shell[WRITE_END]);
                            int status;
                            if (-1 == waitpid(pid, &status, 0))
                                print_error("Failed to await completion of child process", errno);
                            fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n\r", WTERMSIG(status), WEXITSTATUS(status));
                            exit(0);
                        default:
                            write(pipefd_to_shell[WRITE_END], &buf[i], 1);
                        }
                    }
                }
            }

            if (fds[FDS_SHELL_IDX].revents & POLLIN) /* From shell */
            {
                if ((bytes_read = read(pipefd_from_shell[READ_END], &buf, BUF_SIZE)) == -1)
                    print_error("Failed to read output from shell", errno);

                if (compress)
                {
                    if (deflateInit(&socket_stream, Z_DEFAULT_COMPRESSION) != Z_OK)
                    {
                        fprintf(stderr, "Failed to initialize compression on server\n");
                        exit(1);
                    }
                    socket_stream.next_in = (unsigned char *)buf;
                    socket_stream.avail_in = bytes_read;
                    socket_stream.next_out = (unsigned char *)cbuf;
                    socket_stream.avail_out = BUF_SIZE;
                    while (socket_stream.avail_in > 0)
                    {
                        if (deflate(&socket_stream, Z_SYNC_FLUSH) != Z_OK)
                        {
                            fprintf(stderr, "Failed to compress data\n");
                            exit(1);
                        }
                    }

                    write(client_socket_fd, cbuf, BUF_SIZE - socket_stream.avail_out);

                    deflateEnd(&socket_stream);
                }
                else
                {
                    write(client_socket_fd, buf, bytes_read);
                }
            }

            if ((fds[FDS_SOCKET_IDX].revents & (POLLHUP | POLLERR)) || (fds[FDS_SHELL_IDX].revents & (POLLHUP | POLLERR)))
            {
                int status;
                if (-1 == waitpid(pid, &status, 0))
                    print_error("Failed to await completion of child process", errno);
                fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n\r", WTERMSIG(status), WEXITSTATUS(status));
                break;
            }
        }
        if (-1 == poll_ret)
            print_error("Failure on poll", errno);
    }
}

int main(int argc, char *argv[])
{
    static struct option long_options[] = {
        {"port", required_argument, 0, 'p'},
        {"shell", required_argument, 0, 's'},
        {"compress", no_argument, 0, 'c'},
        {0, 0, 0, 0}};

    int ch = 0, option_index = 0;
    int shell = 0, port = 0, compress = 0; /* Flags */
    char *program = "";
    int port_number = -1;

    while ((ch = getopt_long(argc, argv, "", long_options, &option_index)) != -1)
    {
        switch (ch)
        {
        case 'p':
            port = 1;
            port_number = atoi(optarg);
            break;
        case 's':
            shell = 1;
            program = optarg;
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
    if (!port)
    {
        fprintf(stderr, "Must provide --port option.\n");
        exit(1);
    }
    if (!shell)
    {
        fprintf(stderr, "Must provide --shell=program option.\n");
        exit(1);
    }

    // Connect and run client
    int client_socket_fd = connect_to_client(port_number);
    run_server(program, client_socket_fd, compress);

    return 0;
}
