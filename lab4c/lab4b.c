// NAME: Stephanie Doan
// EMAIL: stephaniekdoan@ucla.edu
// ID: 604981556

#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <math.h>
#include <mraa.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define TMP 1
#define BTN 71
#define BUF_SIZE 512
#define B 4275 /* B value of thermistor */
#define R0 100000

FILE *logfile;
mraa_aio_context temp_aio;
mraa_gpio_context button_gpio;
int gen_report = 1; /* turned off by STOP input and on by START input */
int period = 1;     /* sampling interval (seconds) */
char scale = 'F';   /* temperature in Fahrenheit or Celcius*/
struct timeval current_time;
struct tm *time_info;
char print_buf[BUF_SIZE];

void print_error(char *error_string, int errnum, int exit_code)
{
    if (-1 != errnum)
        fprintf(stderr, "%s: %s\n", error_string, strerror(errnum));
    else
        fprintf(stderr, "%s\n", error_string);
    exit(exit_code);
}

void timestamp(char *string)
{
    time_info = localtime(&current_time.tv_sec);
    sprintf(print_buf, "%02d:%02d:%02d %s\n", time_info->tm_hour, time_info->tm_min, time_info->tm_sec, string);
    fprintf(stdout, "%s", print_buf);
    if (logfile != NULL)
        fprintf(logfile, "%s", print_buf);
}

void shutdown()
{
    if (mraa_aio_close(temp_aio) != MRAA_SUCCESS)
        print_error("Failed to close AIO context for temperature", -1, 1);
    if (mraa_gpio_close(button_gpio) != MRAA_SUCCESS)
        print_error("Failed to close GPIO context for button", -1, 1);
    if (logfile != NULL)
        fclose(logfile);
    exit(0);
}

void print_and_shutdown()
{
    timestamp("SHUTDOWN");
    shutdown();
}

void process_input(char *input)
{
    if (strcmp(input, "SCALE=F\n") == 0)
    {
        scale = 'F';
        fprintf(stdout, "SCALE=F\n");
        if (logfile != NULL)
            fprintf(logfile, "SCALE=F\n");
    }
    else if (strcmp(input, "SCALE=C\n") == 0)
    {
        scale = 'C';
        fprintf(stdout, "SCALE=C\n");
        if (logfile != NULL)
            fprintf(logfile, "SCALE=C\n");
    }
    else if (strncmp(input, "PERIOD=", 7) == 0)
    {
        fprintf(stdout, input);
        if (logfile != NULL)
            fprintf(logfile, input);
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
        fprintf(stdout, "STOP\n");
        if (logfile != NULL)
            fprintf(logfile, "STOP\n");
    }
    else if (strcmp(input, "START\n") == 0)
    {
        gen_report = 1;
        fprintf(stdout, "START\n");
        if (logfile != NULL)
            fprintf(logfile, "START\n");
    }
    else if (strncmp(input, "LOG", 3) == 0)
    {
        fprintf(stdout, input);
        if (logfile != NULL)
            fprintf(logfile, input);
    }
    else if (strcmp(input, "OFF\n") == 0)
    {
        fprintf(stdout, "OFF\n");
        if (logfile != NULL)
            fprintf(logfile, "OFF\n");
        print_and_shutdown();
    }
}

int main(int argc, char *argv[])
{
    static struct option long_options[] = {
        {"period", required_argument, 0, 'p'},
        {"scale", required_argument, 0, 's'},
        {"log", required_argument, 0, 'l'},
        {0, 0, 0, 0}};

    int ch = 0, option_index = 0;
    char *filename = "";
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
        default:
            print_error("Invalid argument", -1, 1);
        }
    }

    // Open logfile in response to --log flag
    if (filename != NULL && filename[0] != '\0')
    {
        if ((logfile = fopen(filename, "w")) == NULL)
            print_error("Failed to open log file", -1, 1);
    }

    // Initialize AIO pin for temperature sensor
    if ((temp_aio = mraa_aio_init(TMP)) == NULL)
    {
        mraa_deinit();
        print_error("Failed to initialize AIO for temperature sensor", -1, 1);
    }

    // Initialize GPIO pin for button
    if ((button_gpio = mraa_gpio_init(BTN)) == NULL)
    {
        mraa_deinit();
        print_error("Failed to initialize GPIO for button", -1, 1);
    }
    if (mraa_gpio_dir(button_gpio, MRAA_GPIO_IN) != MRAA_SUCCESS)
        print_error("Failed to configure GPIO infterface to be output pin", -1, 1);
    if (mraa_gpio_isr(button_gpio, MRAA_GPIO_EDGE_RISING, &print_and_shutdown, NULL) != MRAA_SUCCESS)
        print_error("Failed to set [shutdown] to be called when button is pressed", -1, 1);

    // Poll for input from keyboard
    struct pollfd ifd;
    ifd.fd = STDIN_FILENO;
    ifd.events = POLLIN;
    char read_buf[BUF_SIZE];

    float temperature, R;
    gettimeofday(&current_time, /*timezone=*/NULL);
    time_t next_time = current_time.tv_sec + period;
    while (1)
    {
        if (poll(&ifd, /*nfds=*/1, /*timeout=*/0) == -1)
            print_error("Failure on poll", errno, 1);
        else if (ifd.revents & POLLIN)
        {
            if (fgets(read_buf, BUF_SIZE, stdin) == NULL)
                print_error("Failed to read input from keyboard", errno, -1);
            process_input(read_buf);
        }

        if (gettimeofday(&current_time, /*timezone=*/NULL) == -1)
            print_error("Failed to get system clock time", errno, 1);
        if (next_time <= current_time.tv_sec)
        {
            if (gen_report) /* Print time and temperature at interval */
            {
                R = 1023.0 / ((float)mraa_aio_read(temp_aio)) - 1.0;
                R *= R0;
                temperature = 1.0 / (log(R / R0) / B + 1 / 298.15) - 273.15; /* celcius */
                if (scale == 'F')
                    temperature = temperature * 9 / 5 + 32;
                char temperature_string[32];
                sprintf(temperature_string, "%.1f", temperature);
                timestamp(temperature_string);
            }

            next_time = current_time.tv_sec + period;
        }
    }

    shutdown();
    return 0;
}