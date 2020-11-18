// NAME: Stephanie Doan
// EMAIL: stephaniekdoan@ucla.edu
// ID: 604981556

#include "SortedList.h"
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define PRIME1 38971
#define PRIME2 90053

int iterations = 1;
int lists = 1;
int opt_yield = 0;
int opt_sync = '\0';
SortedList_t *list_arr;
SortedListElement_t *list_els;
pthread_mutex_t *mutex_arr; /* sync with mutex */
long long *mutex_wait_times;
int *spin_lock_arr; /* sync with spin lock */

void print_error(char *error_string, int errnum, int exit_code)
{
    if (-1 != errnum)
        fprintf(stderr, "%s: %s\n", error_string, strerror(errnum));
    else
        fprintf(stderr, "%s\n", error_string);
    exit(exit_code);
}

void sighandler(int num)
{
    print_error("Run failed, caught signal", num, 1);
}

int get_list_idx(const char *key)
{
    int hash = 19; /* prime number */
    while (*key)
    {
        hash = (hash ^ PRIME1) ^ (key[0] * PRIME2);
        key++;
    }
    return hash % lists;
}

void *thread_list(void *start_el)
{
    // Timing mutex synchronization wait
    long long total_wait = 0; /* ns */
    struct timespec start_ts, end_ts;

    // Insert list elements
    SortedListElement_t *start = (SortedListElement_t *)start_el;
    int i;
    for (i = 0; i < iterations; ++i)
    {
        int list_idx = get_list_idx((start + i)->key);
        if (opt_sync == 'm')
        {
            if (-1 == clock_gettime(CLOCK_MONOTONIC, &start_ts))
                print_error("Failed to retrieve start time", errno, 1);
            pthread_mutex_lock(&mutex_arr[list_idx]);
            if (-1 == clock_gettime(CLOCK_MONOTONIC, &end_ts))
                print_error("Failed to retrieve end time", errno, 1);
            total_wait += (end_ts.tv_sec - start_ts.tv_sec) * 1000000000 + (end_ts.tv_nsec - start_ts.tv_nsec);
            SortedList_insert(&list_arr[list_idx], start + i);
            pthread_mutex_unlock(&mutex_arr[list_idx]);
        }
        else if (opt_sync == 's')
        {
            while (__sync_lock_test_and_set(&spin_lock_arr[list_idx], 1))
                ;
            SortedList_insert(&list_arr[list_idx], start + i);
            __sync_lock_release(&spin_lock_arr[list_idx]);
        }
        else
        {
            SortedList_insert(&list_arr[list_idx], start + i);
        }
    }

    // Get length of each list
    for (i = 0; i < iterations; ++i)
    {
        int list_idx = get_list_idx((start + i)->key);
        if (opt_sync == 'm')
        {
            if (-1 == clock_gettime(CLOCK_MONOTONIC, &start_ts))
                print_error("Failed to retrieve start time", errno, 1);
            pthread_mutex_lock(&mutex_arr[list_idx]);
            if (-1 == clock_gettime(CLOCK_MONOTONIC, &end_ts))
                print_error("Failed to retrieve end time", errno, 1);
            total_wait += (end_ts.tv_sec - start_ts.tv_sec) * 1000000000 + (end_ts.tv_nsec - start_ts.tv_nsec);
            SortedList_length(&list_arr[list_idx]);
            pthread_mutex_unlock(&mutex_arr[list_idx]);
        }
        else if (opt_sync == 's')
        {
            while (__sync_lock_test_and_set(&spin_lock_arr[list_idx], 1))
                ;
            SortedList_length(&list_arr[list_idx]);
            __sync_lock_release(&spin_lock_arr[list_idx]);
        }
        else
        {
            SortedList_length(&list_arr[list_idx]);
        }
    }

    // Look up and delete inserted keys
    SortedListElement_t *to_delete = NULL;
    for (i = 0; i < iterations; ++i)
    {
        int list_idx = get_list_idx((start + i)->key);
        if (opt_sync == 'm')
        {
            if (-1 == clock_gettime(CLOCK_MONOTONIC, &start_ts))
                print_error("Failed to retrieve start time", errno, 1);
            pthread_mutex_lock(&mutex_arr[list_idx]);
            if (-1 == clock_gettime(CLOCK_MONOTONIC, &end_ts))
                print_error("Failed to retrieve end time", errno, 1);
            total_wait += (end_ts.tv_sec - start_ts.tv_sec) * 1000000000 + (end_ts.tv_nsec - start_ts.tv_nsec);
            to_delete = SortedList_lookup(&list_arr[list_idx], (start + i)->key);
            if (!to_delete)
                print_error("Key can not be found in list", -1, 2);
            if (1 == SortedList_delete(to_delete))
                print_error("Failed to delete element from list", -1, 2);
            pthread_mutex_unlock(&mutex_arr[list_idx]);
        }
        else if (opt_sync == 's')
        {
            while (__sync_lock_test_and_set(&spin_lock_arr[list_idx], 1))
                ;
            to_delete = SortedList_lookup(&list_arr[list_idx], (start + i)->key);
            if (!to_delete)
                print_error("Key can not be found in list", -1, 2);
            if (1 == SortedList_delete(to_delete))
                print_error("Failed to delete element from list", -1, 2);
            __sync_lock_release(&spin_lock_arr[list_idx]);
        }
        else
        {
            to_delete = SortedList_lookup(&list_arr[list_idx], (start + i)->key);
            if (!to_delete)
                print_error("Key can not be found in list", -1, 2);
            if (1 == SortedList_delete(to_delete))
                print_error("Failed to delete element from list", -1, 2);
        }
    }

    if (opt_sync == 'm')
    {
        long long thread_idx = ((SortedListElement_t *)start_el - list_els) / iterations;
        mutex_wait_times[thread_idx] = total_wait;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    signal(SIGSEGV, sighandler);

    static struct option long_options[] = {
        {"threads", required_argument, 0, 't'},
        {"iterations", required_argument, 0, 'i'},
        {"yield", required_argument, 0, 'y'},
        {"sync", required_argument, 0, 's'},
        {"lists", required_argument, 0, 'l'},
        {0, 0, 0, 0}};

    int ch = 0, option_index = 0, i = 0;
    int threads = 1;
    char *yieldopts = "none", *syncopts = "none";
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
            for (i = 0; i < (int)strlen(optarg); ++i)
            {
                if (optarg[i] == 'i')
                    opt_yield |= INSERT_YIELD;
                else if (optarg[i] == 'd')
                    opt_yield |= DELETE_YIELD;
                else if (optarg[i] == 'l')
                    opt_yield |= LOOKUP_YIELD;
                else
                    print_error("Invalid argument to --yield flag", -1, 1);
            }
            yieldopts = optarg;
            break;
        case 's':
            if (strlen(optarg) == 1 && (optarg[0] == 'm' || optarg[0] == 's'))
            {
                opt_sync = optarg[0];
                syncopts = optarg;
            }
            else
                print_error("Invalid argument to --sync flag", -1, 1);
            break;
        case 'l':
            lists = atoi(optarg);
            break;
        default:
            print_error("Invalid argument", -1, 1);
        }
    }

    // Initialize [threads * iterations] list elements
    int els = threads * iterations;
    list_els = (SortedListElement_t *)malloc(els * sizeof(SortedListElement_t));
    for (i = 0; i < els; ++i)
    {
        // Random character between ASCII 32 (space) and 126 (~)
        char *rand_char = malloc(sizeof(char));
        *rand_char = (rand() % (126 - 32)) + 32;
        (list_els + i)->key = rand_char;
    }
    // Initialize array of [lists] sorted lists
    list_arr = malloc(lists * sizeof(SortedList_t));

    // If mutex synchronized, initialize array of size [threads] to store total mutex times
    if (opt_sync == 'm')
    {
        mutex_arr = malloc(lists * sizeof(pthread_mutex_t));
        mutex_wait_times = calloc(threads, sizeof(long long));
    }
    if (opt_sync == 's')
        spin_lock_arr = malloc(lists * sizeof(int));

    // Start clock
    struct timespec start_ts, end_ts;
    if (-1 == clock_gettime(CLOCK_MONOTONIC, &start_ts))
        print_error("Failed to retrieve start time", errno, 1);

    // Run threads
    pthread_t *thread_arr = malloc(sizeof(pthread_t) * threads);
    for (i = 0; i < threads; ++i)
    {
        if (0 != pthread_create(/*thread=*/&thread_arr[i], /*attr=*/NULL, thread_list, (void *)&list_els[i * iterations]))
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
    int ops = 3 * els;
    long long avg_time = total_time / ops;
    long long mutex_avg_wait = 0;
    if (opt_sync == 'm')
    {
        long long mutex_total = 0;
        for (i = 0; i < threads; ++i)
            mutex_total += mutex_wait_times[i];
        mutex_avg_wait = mutex_total / ops;
    }

    // Check that length of each list in list_arr is 0
    for (i = 0; i < lists; ++i)
    {
        if (0 != SortedList_length(&list_arr[i]))
            print_error("Length of a list is not 0", -1, 2);
    }

    // Log test
    printf("list-%s-%s,%d,%d,%d,%d,%lld,%lld,%lld\n", yieldopts, syncopts, threads, iterations, lists, ops, total_time, avg_time, mutex_avg_wait);

    free(thread_arr);
    for (i = 0; i < els; ++i)
        free((char *)((list_els + i)->key));
    free(list_els);
    if (opt_sync == 'm')
    {
        free(mutex_arr);
        free(mutex_wait_times);
    }
    if (opt_sync == 's')
        free(spin_lock_arr);
    return 0;
}
