// NAME: Stephanie Doan
// EMAIL: stephaniekdoan@ucla.edu
// ID: 604981556

#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <math.h>
// #include <mraa.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define TMP 1
#define BUF_SIZE 512
#define B 4275 /* B value of thermistor */
#define R0 100000

// mraa_aio_context temp_aio;

int gen_report = 1; /* turned off by STOP input and on by START input */
int period = 1;     /* sampling interval (seconds) */
char scale = 'F';   /* temperature in Fahrenheit or Celcius*/
FILE *logfile;      /* log file */

int socket_fd = -1;
struct timeval current_time;
struct tm *time_info;
char print_buf[BUF_SIZE];

SSL_CTX *context = NULL;
SSL *ssl_client = NULL;

void print_error(char *error_string, int errnum, int exit_code)
{
    if (-1 != errnum)
        fprintf(stderr, "%s: %s\n", error_string, strerror(errnum));
    else
        fprintf(stderr, "%s\n", error_string);
    exit(exit_code);
}

void log_to_file(char *string)
{
    fprintf(stdout, "%s", string);
    if (logfile != NULL)
        fprintf(logfile, "%s", string);
}

void send_and_log(char *string)
{
    int ret = SSL_write(ssl_client, string, strlen(string));
    if (ret <= 0)
        print_error("Failed to write to SSL connection", SSL_get_error(ssl_client, ret), 2);
    log_to_file(string);
}

void timestamp(char *string)
{
    time_info = localtime(&current_time.tv_sec);
    sprintf(print_buf, "%02d:%02d:%02d %s\n", time_info->tm_hour, time_info->tm_min, time_info->tm_sec, string);
    send_and_log(print_buf);
}

void shut_down()
{
    // if (mraa_aio_close(temp_aio) != MRAA_SUCCESS)
    //     print_error("Failed to close AIO context for temperature", -1, 1);
    if (logfile != NULL)
        fclose(logfile);
    exit(0);
}

void print_and_shutdown()
{
    timestamp("SHUTDOWN");
    shut_down();
}

void process_input(char *input)
{
    if (strcmp(input, "SCALE=F\n") == 0)
    {
        scale = 'F';
        log_to_file(input);
    }
    else if (strcmp(input, "SCALE=C\n") == 0)
    {
        scale = 'C';
        log_to_file(input);
    }
    else if (strncmp(input, "PERIOD=", 7) == 0)
    {
        log_to_file(input);
        int new_period = 0;
        size_t i;
        for (i = 7; i < strlen(input); ++i)
        {
            if (!isdigit(input[i]))
                break;
            new_period = 10 * new_period + ((int)(input[i] - '0'));
        }
        period = new_period;
    }
    else if (strcmp(input, "STOP\n") == 0)
    {
        gen_report = 0;
        log_to_file(input);
    }
    else if (strcmp(input, "START\n") == 0)
    {
        gen_report = 1;
        log_to_file(input);
    }
    else if (strncmp(input, "LOG", 3) == 0)
    {
        log_to_file(input);
    }
    else if (strcmp(input, "OFF\n") == 0)
    {
        log_to_file(input);
        print_and_shutdown();
    }
}

// Function adapted from my code for project 1B
int connect_to_server(char host[], int port)
{
    struct hostent *h = gethostbyname(host);
    if (h == NULL)
        print_error("Failed get host by name", errno, 1);

    int socket_fd = socket(/*domain=*/AF_INET, /*type=*/SOCK_STREAM, /*protocol=*/0);
    if (-1 == socket_fd)
        print_error("Failed to open socket on client", errno, 1);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    memcpy(&(server_address.sin_addr.s_addr), h->h_addr, h->h_length);
    if (-1 == connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)))
        print_error("Failed to connect client to server", errno, 1);

    return socket_fd;
}

void initialize_ssl()
{
    OpenSSL_add_all_algorithms();
    SSL_library_init();
    SSL_load_error_strings();

    // Create context (configuration) object (SSL_CTX)
    context = SSL_CTX_new(TLSv1_client_method());
    if (context == NULL)
        print_error("Failed to create SSL context object", -1, 2);

    // Use configuration object to create SSL client (SSL)
    ssl_client = SSL_new(context);
    if (ssl_client == NULL)
        print_error("Failed to create SSL client", -1, 2);

    // Associate TCP socket with SSL client
    if (SSL_set_fd(ssl_client, socket_fd) != 1)
        print_error("Failed to associate TCP socket with SSL client", -1, 2);

    // Make SSL connection with SSL client
    int ret = SSL_connect(ssl_client);
    if (ret != 1)
        print_error("Failed to make SSL connection with SSL client", SSL_get_error(ssl_client, ret), 2);
}

int main(int argc, char *argv[])
{
    static struct option long_options[] = {
        {"period", required_argument, 0, 'p'},
        {"scale", required_argument, 0, 's'},
        {"log", required_argument, 0, 'l'},
        {"id", required_argument, 0, 'i'},
        {"host", required_argument, 0, 'h'},
        {0, 0, 0, 0}};

    int ch = 0, option_index = 0;
    char *filename = ""; /* name for log file */
    char *hostname = ""; /* host name or address */
    char *id = "";       /* 0-digit number */
    int port_number = -1;
    while ((ch = getopt_long(argc, argv, "", long_options, &option_index)) != -1)
    {
        switch (ch)
        {
        case 'p':
            period = atoi(optarg);
            break;
        case 's':
            if (strlen(optarg) == 1 && (optarg[0] == 'C' || optarg[0] == 'F'))
                scale = optarg[0];
            else
                print_error("Invalid argument to --scale flag", -1, 1);
            break;
        case 'l':
            filename = optarg;
            break;
        case 'i':
            id = optarg;
            break;
        case 'h':
            hostname = optarg;
            break;
        default:
            print_error("Invalid argument", -1, 1);
        }
    }
    // Port number
    if (optind == argc - 1)
        port_number = atoi(argv[optind]);
    else
        print_error("Invalid number of arguments, cannot read port number", -1, 1);

    // Open logfile in response to --log flag
    if (filename != NULL && filename[0] != '\0')
    {
        if ((logfile = fopen(filename, "w")) == NULL)
            print_error("Failed to open log file", -1, 1);
    }

    // Open a TLS connection to the server and initialize SSL
    socket_fd = connect_to_server(hostname, port_number);
    initialize_ssl();

    // Send and log ID=ID-number
    sprintf(print_buf, "ID=%s\n", id);
    send_and_log(print_buf);

    // Initialize AIO pin for temperature sensor
    // if ((temp_aio = mraa_aio_init(TMP)) == NULL)
    // {
    //     mraa_deinit();
    //     print_error("Failed to initialize AIO for temperature sensor", -1, 1);
    // }

    // Poll for input from server, process and log newline-terminated commands
    struct pollfd ifd;
    ifd.fd = socket_fd;
    ifd.events = POLLIN;
    char read_buf[BUF_SIZE];
    int bytes_read = -1;

    float temperature; // , R;
    gettimeofday(&current_time, /*timezone=*/NULL);
    time_t next_time = current_time.tv_sec + period;
    while (1)
    {
        if (poll(&ifd, /*nfds=*/1, /*timeout=*/0) == -1)
            print_error("Failure on poll", errno, 1);
        else if (ifd.revents & POLLIN)
        {
            if ((bytes_read = SSL_read(ssl_client, read_buf, BUF_SIZE)) == -1)
                print_error("Failed to read input from server", errno, -1);
            int command_length = 0, i = 0;
            while (i < bytes_read && read_buf[i] != '\n')
            {
                ++command_length;
                ++i;
            }
            char *command_buf = malloc(command_length + 2);
            memcpy(command_buf, read_buf, command_length);
            command_buf[command_length] = '\n';
            command_buf[command_length + 1] = '\0';
            process_input(command_buf);
            free(command_buf);
        }

        if (gettimeofday(&current_time, /*timezone=*/NULL) == -1)
            print_error("Failed to get system clock time", errno, 1);
        if (next_time <= current_time.tv_sec)
        {
            if (gen_report) /* Print time and temperature at interval */
            {
                // Send and log newline terminated temperature reports
                // R = 1023.0 / ((float)mraa_aio_read(temp_aio)) - 1.0;
                // R *= R0;
                // temperature = 1.0 / (log(R / R0) / B + 1 / 298.15) - 273.15; /* celcius */
                temperature = 40;
                if (scale == 'F')
                    temperature = temperature * 9 / 5 + 32;
                char temperature_string[32];
                sprintf(temperature_string, "%.1f", temperature);
                timestamp(temperature_string);
            }

            next_time = current_time.tv_sec + period;
        }
    }

    shut_down();
    return 0;
}
