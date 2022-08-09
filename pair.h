#ifndef __PAIR_H
#define __PAIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/memory.h"

struct pair
{
    void* first;
    void* second;

    size_t of_size_first;
    size_t of_size_second;
};

struct pair* pair_create(size_t of_size_first, size_t of_size_second)
{
    if (of_size_first == 0 || of_size_second == 0)
    {
        fprintf(stderr, "stderr: Size of a single element of pair cannot be zero.\n");
        abort();
    }

    struct pair* ths = (struct pair*)malloc_s(sizeof(struct pair));

    ths->first = malloc_s(of_size_first);
    ths->second = malloc_s(of_size_second);
    if (ths->first == NULL || ths->second == NULL)
    {
        fprintf(stderr, "stderr: Failed to realloc_sate memory for pair in pair_create().\n");
        abort();
    }

    ths->of_size_first = of_size_first;
    ths->of_size_second = of_size_second;

    return ths;
}

struct pair* pair_make(void* first, size_t of_size_first, void* second, size_t of_size_second)
{
    if (first == NULL || second == NULL)
    {
        fprintf(stderr, "stderr: Pair element cannot be NULL.\n");
        abort();
    }

    struct pair* ths = pair_create(of_size_first, of_size_second);

    memcpy(ths->first, first, ths->of_size_first);
    memcpy(ths->second, second, ths->of_size_second);

    return ths;
}

void pair_delete(struct pair* ths)
{
    free_s(ths->first);
    free_s(ths->second);
    free_s(ths);
}

void pair_get_first(struct pair* ths, void* dest)
{
    memcpy(dest, ths->first, ths->of_size_first);
}

void pair_get_second(struct pair* ths, void* dest)
{
    memcpy(dest, ths->second, ths->of_size_second);
}

void pair_set_first(struct pair* ths, void* value)
{
    memcpy(ths->first, value, ths->of_size_first);
}

void pair_set_second(struct pair* ths, void* value)
{
    memcpy(ths->second, value, ths->of_size_second);
}

#endif