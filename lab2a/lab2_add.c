// NAME: Stephanie Doan
// EMAIL: stephaniekdoan@ucla.edu
// ID: 604981556

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

int iterations = 1;
int opt_yield = 0;
int opt_sync = '\0';
pthread_mutex_t mutex; /* sync with mutex */
volatile int lock = 0; /* sync with spin lock */

void print_error(char *error_string, int errnum, int exit_code)
{
    if (-1 != errnum)
        fprintf(stderr, "%s: %s\n", error_string, strerror(errnum));
    else
        fprintf(stderr, "%s\n", error_string);
    exit(exit_code);
}

void add(long long *pointer, long long value)
{
    long long sum = *pointer + value;
    if (opt_yield)
        sched_yield();
    *pointer = sum;
}

void add_none(long long *counter)
{
    int i;
    for (i = 0; i < iterations; ++i)
        add((long long *)counter, 1);
    for (i = 0; i < iterations; ++i)
        add((long long *)counter, -1);
}

void add_mutex(long long *counter)
{
    int i;
    for (i = 0; i < iterations; ++i)
    {
        pthread_mutex_lock(&mutex);
        add((long long *)counter, 1);
        pthread_mutex_unlock(&mutex);
    }
    for (i = 0; i < iterations; ++i)
    {
        pthread_mutex_lock(&mutex);
        add((long long *)counter, -1);
        pthread_mutex_unlock(&mutex);
    }
}

void add_spin(long long *counter)
{
    int i;
    for (i = 0; i < iterations; ++i)
    {
        while (__sync_lock_test_and_set(&lock, 1))
            ;
        add((long long *)counter, 1);
        __sync_lock_release(&lock);
    }
    for (i = 0; i < iterations; ++i)
    {
        while (__sync_lock_test_and_set(&lock, 1))
            ;
        add((long long *)counter, -1);
        __sync_lock_release(&lock);
    }
}

void add_cas(long long *counter)
{
    int i;
    long long old_val;
    for (i = 0; i < iterations; ++i)
    {
        do
        {
            old_val = *counter;
            if (opt_yield)
                sched_yield();
        } while (__sync_val_compare_and_swap(counter, old_val, old_val + 1) != old_val);
    }
    for (i = 0; i < iterations; ++i)
    {
        do
        {
            old_val = *counter;
            if (opt_yield)
                sched_yield();
        } while (__sync_val_compare_and_swap(counter, old_val, old_val - 1) != old_val);
    }
}

void *thread_add(void *counter)
{
    switch (opt_sync)
    {
    case '\0':
        add_none((long long *)counter);
        break;
    case 'm':
        add_mutex((long long *)counter);
        break;
    case 's':
        add_spin((long long *)counter);
        break;
    case 'c':
        add_cas((long long *)counter);
        break;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    static struct option long_options[] = {
        {"threads", required_argument, 0, 't'},
        {"iterations", required_argument, 0, 'i'},
        {"yield", no_argument, 0, 'y'},
        {"sync", required_argument, 0, 's'},
        {0, 0, 0, 0}};

    int ch = 0, option_index = 0;
    int threads = 1;
    while ((ch = getopt_long(argc, argv, "", long_options, &option_index)) != -1)
    {
        switch (ch)
        {
        case 't':
            threads = atoi(optarg);
            break;
        case 'i':
            iterations = atoi(optarg);
            break;
        case 'y':
            opt_yield = 1;
            break;
        case 's':
            if (strlen(optarg) == 1 && (optarg[0] == 'm' || optarg[0] == 's' || optarg[0] == 'c'))
                opt_sync = optarg[0];
            else
                print_error("Invalid argument to --sync flag", -1, 1);
            break;
        default:
            print_error("Invalid argument", -1, 1);
        }
    }

    // Start clock
    struct timespec start_ts, end_ts;
    if (-1 == clock_gettime(CLOCK_MONOTONIC, &start_ts))
        print_error("Failed to retrieve start time", errno, 1);

    // Run threads
    long long counter = 0;
    pthread_t *thread_arr = malloc(sizeof(pthread_t) * threads);
    int i;
    for (i = 0; i < threads; ++i)
    {
        if (0 != pthread_create(/*thread=*/&thread_arr[i], /**attr=*/NULL, thread_add, &counter))
            print_error("Failed to create thread", errno, 1);
    }
    for (i = 0; i < threads; ++i)
    {
        if (0 != pthread_join(/*thread=*/thread_arr[i], NULL))
            print_error("Failed to wait for thread to terminate", errno, 1);
    }

    // End clock
    if (-1 == clock_gettime(CLOCK_MONOTONIC, &end_ts))
        print_error("Failed to retrieve end time", errno, 1);
    long long total_time = (end_ts.tv_sec - start_ts.tv_sec) * 1000000000 + (end_ts.tv_nsec - start_ts.tv_nsec);
    int ops = 2 * threads * iterations;
    long long avg_time = total_time / ops;

    // Test name
    char test_name[20] = "add";
    if (opt_yield)
        strcat(test_name, "-yield");
    if (opt_sync != '\0')
    {
        char sync_str[] = {'-', (char)opt_sync, '\0'};
        strcat(test_name, sync_str);
    }
    else
        strcat(test_name, "-none");

    printf("%s,%d,%d,%d,%lld,%lld,%lld\n", test_name, threads, iterations, ops, total_time, avg_time, counter);

    free(thread_arr);

    return 0;
}
