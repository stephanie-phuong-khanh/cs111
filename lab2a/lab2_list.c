// NAME: Stephanie Doan
// EMAIL: stephaniekdoan@ucla.edu
// ID: 604981556

#include "SortedList.h"
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
SortedList_t list;
SortedListElement_t *list_els;
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

void *thread_list(void *start_el)
{
    // Insert list elements
    SortedListElement_t *start = (SortedListElement_t *)start_el;
    int i;
    for (i = 0; i < iterations; ++i)
    {
        if (opt_sync == 'm')
        {
            pthread_mutex_lock(&mutex);
            SortedList_insert(&list, start + i);
            pthread_mutex_unlock(&mutex);
        }
        else if (opt_sync == 's')
        {
            while (__sync_lock_test_and_set(&lock, 1))
                ;
            SortedList_insert(&list, start + i);
            __sync_lock_release(&lock);
        }
        else
        {
            SortedList_insert(&list, start + i);
        }
    }

    // Get list length
    if (opt_sync == 'm')
    {
        pthread_mutex_lock(&mutex);
        SortedList_length(&list);
        pthread_mutex_unlock(&mutex);
    }
    else if (opt_sync == 's')
    {
        while (__sync_lock_test_and_set(&lock, 1))
            ;
        SortedList_length(&list);
        __sync_lock_release(&lock);
    }
    else
        SortedList_length(&list);

    // Look up and delete inserted keys
    SortedListElement_t *to_delete = NULL;
    for (i = 0; i < iterations; ++i)
    {
        if (opt_sync == 'm')
        {
            pthread_mutex_lock(&mutex);
            to_delete = SortedList_lookup(&list, (start + i)->key);
            if (!to_delete)
                print_error("Key can not be found in list", -1, 2);
            if (1 == SortedList_delete(to_delete))
                print_error("Failed to delete element from list", -1, 2);
            pthread_mutex_unlock(&mutex);
        }
        else if (opt_sync == 's')
        {
            while (__sync_lock_test_and_set(&lock, 1))
                ;
            to_delete = SortedList_lookup(&list, (start + i)->key);
            if (!to_delete)
                print_error("Key can not be found in list", -1, 2);
            if (1 == SortedList_delete(to_delete))
                print_error("Failed to delete element from list", -1, 2);
            __sync_lock_release(&lock);
        }
        else
        {
            to_delete = SortedList_lookup(&list, (start + i)->key);
            if (!to_delete)
                print_error("Key can not be found in list", -1, 2);
            if (1 == SortedList_delete(to_delete))
                print_error("Failed to delete element from list", -1, 2);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    static struct option long_options[] = {
        {"threads", required_argument, 0, 't'},
        {"iterations", required_argument, 0, 'i'},
        {"yield", required_argument, 0, 'y'},
        {"sync", required_argument, 0, 's'},
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

    // Start clock
    struct timespec start_ts, end_ts;
    if (-1 == clock_gettime(CLOCK_MONOTONIC, &start_ts))
        print_error("Failed to retrieve start time", errno, 1);

    // Run threads
    pthread_t *thread_arr = malloc(sizeof(pthread_t) * threads);
    for (i = 0; i < threads; ++i)
    {
        if (0 != pthread_create(/*thread=*/&thread_arr[i], /**attr=*/NULL, thread_list, (void *)&list_els[i * iterations])) /* Fill in arguments */
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

    // Check that length of list is 0
    if (0 != SortedList_length(&list))
        print_error("Length of list is not 0", -1, 2);

    // Log test
    printf("list-%s-%s,%d,%d,1,%d,%lld,%lld\n", yieldopts, syncopts, threads, iterations, ops, total_time, avg_time);

    free(thread_arr);
    for (i = 0; i < els; ++i)
        free((char *)((list_els + i)->key));
    free(list_els);
    return 0;
}
