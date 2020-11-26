// NAME: Stephanie Doan
// EMAIL: stephaniekdoan@ucla.edu
// ID: 604981556

#include <errno.h>
#include <getopt.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#define CR 0x0D
#define LF 0x0A
#define CTRL_C 0x03
#define CTRL_D 0x04

struct termios termios_orig;
static char cr_lf[2] = {CR, LF}, caret_c[2] = "^C", caret_d[2] = "^D";
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

void exec_shell(char *program)
{
    int pipefd_to_shell[2], pipefd_from_shell[2];
    static const int READ_END = 0, WRITE_END = 1;

    if (-1 == pipe(pipefd_to_shell) || -1 == pipe(pipefd_from_shell))
        print_error("Failed to create pipe", errno);

    int pid = fork();
    if (-1 == pid)
        print_error("Failed to create new process with fork", errno);
    if (0 == pid)
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
    else
    {
        close(pipefd_to_shell[READ_END]);
        close(pipefd_from_shell[WRITE_END]);

        static const int NFDS = 2, FDS_KEY_IDX = 0, FDS_SHELL_IDX = 1;
        struct pollfd fds[NFDS];
        fds[FDS_KEY_IDX].fd = STDIN_FILENO; /* Reads input from keyboard */
        fds[FDS_KEY_IDX].events = POLLIN;
        fds[FDS_SHELL_IDX].fd = pipefd_from_shell[READ_END]; /* Reads input from shell */
        fds[FDS_SHELL_IDX].events = POLLIN;

        int poll_ret = 0, bytes_read = 0;
        char buf[BUF_SIZE];
        while ((poll_ret = poll(fds, NFDS, /*timeout=*/0)) != -1)
        {
            if (fds[FDS_KEY_IDX].revents & POLLIN)
            {
                if ((bytes_read = read(STDIN_FILENO, &buf, BUF_SIZE)) == -1)
                    print_error("Failed to read input from terminal", errno);
                int i;
                for (i = 0; i < bytes_read; ++i)
                {
                    switch (buf[i])
                    {
                    case CR:
                    case LF:
                        if (-1 == write(STDOUT_FILENO, &cr_lf, 2))
                            print_error("Failed to echo <cr><lf> to terminal", errno);
                        if (-1 == write(pipefd_to_shell[WRITE_END], &cr_lf[1], 1))
                            print_error("Failed to write <lf> to shell", errno);
                        break;
                    case CTRL_D:
                        if (-1 == write(STDOUT_FILENO, &caret_d, 2))
                            print_error("Failed to echo ^D to terminal", errno);
                        close(pipefd_to_shell[WRITE_END]); /* Close pipe to shell */
                        int status;
                        if (-1 == waitpid(pid, &status, 0))
                            print_error("Failed to await shell process's completion", errno);
                        fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n\r", WTERMSIG(status), WEXITSTATUS(status));
                        exit(0);
                    case CTRL_C:
                        if (-1 == write(STDOUT_FILENO, &caret_c, 2))
                            print_error("Failed to echo ^C to terminal", errno);
                        if (-1 == kill(pid, SIGINT))
                            print_error("Failed to send SIGINT to shell", errno);
                        break;
                    default:
                        if (-1 == write(STDOUT_FILENO, &buf[i], 1))
                            print_error("Failed to echo input to terminal", errno);
                        if (-1 == write(pipefd_to_shell[WRITE_END], &buf[i], 1))
                            print_error("Failed to write input to shell", errno);
                    }
                }
            }

            if (fds[FDS_SHELL_IDX].revents & POLLIN)
            {
                if ((bytes_read = read(pipefd_from_shell[READ_END], &buf, BUF_SIZE)) == -1)
                    print_error("Failed to read output from shell", errno);
                int i;
                for (i = 0; i < bytes_read; ++i)
                {
                    switch (buf[i])
                    {
                    case CR:
                    case LF:
                        if (-1 == write(STDOUT_FILENO, &cr_lf, 2))
                            print_error("Failed to write <cr><lf> from shell to terminal", errno);
                        break;
                    default:
                        if (-1 == write(STDOUT_FILENO, &buf[i], 1))
                            print_error("Failed to write output from shell to terminal", errno);
                    }
                }
            }

            if ((fds[FDS_KEY_IDX].revents & (POLLHUP | POLLERR)) || (fds[FDS_SHELL_IDX].revents & (POLLHUP | POLLERR)))
            {
                int status;
                if (-1 == waitpid(pid, &status, 0))
                    print_error("Failed to await shell process's completion", errno);
                fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n\r", WTERMSIG(status), WEXITSTATUS(status));
                break;
            }
        }
    }
}

void no_shell()
{
    char buf[BUF_SIZE];
    int bytes_read = 0;
    while ((bytes_read = read(STDIN_FILENO, &buf, BUF_SIZE)) > 0)
    {
        int i;
        for (i = 0; i < bytes_read; ++i)
        {
            switch (buf[i])
            {
            case CR:
            case LF:
                if (-1 == write(STDOUT_FILENO, &cr_lf, 2))
                    print_error("Failed to write <cr><lf> to terminal", errno);
                break;
            case CTRL_D:
                exit(0);
            default:
                if (-1 == write(STDOUT_FILENO, &buf[i], 1))
                    print_error("Failed to write to terminal", errno);
            }
        }
    }
    if (-1 == bytes_read)
        print_error("Failed to read input from terminal", errno);
}

int main(int argc, char *argv[])
{
    init_terminal();
    atexit(restore_terminal);
    if (-1 == atexit(restore_terminal))
        print_error("Failed to register terminal restore function to be called at process termination", errno);

    static struct option long_options[] = {
        {"shell", required_argument, 0, 's'},
        {0, 0, 0, 0}};

    int ch = 0, option_index = 0, shell = 0;
    char *program = "";
    while ((ch = getopt_long(argc, argv, "", long_options, &option_index)) != -1)
    {
        switch (ch)
        {
        case 's':
            shell = 1;
            program = optarg;
            break;
        default:
            fprintf(stderr, "Invalid argument\n");
            exit(1);
        }
    }

    if (shell)
        exec_shell(program);
    else
        no_shell();

    return 0;
}
