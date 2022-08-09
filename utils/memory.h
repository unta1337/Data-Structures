#ifndef __MEMORY_H
#define __MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t used_malloc = 0;

void* malloc_s(size_t size)
{
    void* buffer = malloc(size + sizeof(size_t));
    if (buffer == NULL)
        return NULL;

    used_malloc += size;

    size_t* size_box = (size_t*)buffer;
    *size_box = size;

    return buffer + sizeof(size_t);
}

void* realloc_s(void* ptr, size_t size)
{
    void* buffer = ptr - sizeof(size_t);
    size_t* size_box = (size_t*)buffer;

    used_malloc -= *size_box;

    buffer = realloc(buffer, size + sizeof(size_t));
    if (buffer == NULL)
        return NULL;

    used_malloc += size;
    *size_box = size;

    return buffer + sizeof(size_t);
}

void free_s(void* ptr)
{
    if (ptr == NULL)
        return;

    void* buffer = ptr - sizeof(size_t);
    size_t* size_box = (size_t*)buffer;

    used_malloc -= *size_box;

    free(buffer);
}

#endif