// NAME: Stephanie Doan
// EMAIL: stephaniekdoan@ucla.edu
// ID: 604981556

#include "SortedList.h"
#include <string.h>
#include <sched.h>
#include <stdio.h>

void SortedList_insert(SortedList_t *list, SortedListElement_t *element)
{
    if (!list || !element)
        return;

    if (!list->next)
    {
        if (opt_yield & INSERT_YIELD)
            sched_yield();
        list->next = list;
        list->prev = list;
        list->key = NULL;
    }

    SortedList_t *cur = list->next;
    while (cur != list && *(element->key) > *(cur->key))
        cur = cur->next;

    if (opt_yield & INSERT_YIELD)
        sched_yield();

    cur->prev->next = element;
    element->prev = cur->prev;
    element->next = cur;
    cur->prev = element;
}

int SortedList_delete(SortedListElement_t *element)
{
    if (!element || element->next->prev != element || element->prev->next != element)
        return 1;

    if (opt_yield & DELETE_YIELD)
        sched_yield();

    element->next->prev = element->prev;
    element->prev->next = element->next;
    return 0;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key)
{
    if (!list || !list->next)
        return NULL;

    SortedList_t *cur = list->next;
    while (cur != list)
    {
        if (key == cur->key)
            return cur;
        if (opt_yield & LOOKUP_YIELD)
            sched_yield();
        cur = cur->next;
    }
    return NULL;
}

int SortedList_length(SortedList_t *list)
{
    if (!list)
        return -1;
    int length = 0;
    SortedList_t *cur = list->next;
    while (cur != list)
    {
        ++length;
        if (opt_yield & LOOKUP_YIELD)
            sched_yield();
        cur = cur->next;
    }
    return length;
}
