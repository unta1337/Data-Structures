#ifndef _LIST_H
#define _LIST_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct list
{
    void* arr;
    size_t capacity;
    size_t of_size;

    size_t size;
};

struct list* list_create(size_t of_size)
{
    struct list* ths = (struct list*)malloc(sizeof(struct list));

    ths->arr = malloc(of_size);
    ths->capacity = 1;
    ths->of_size = of_size;

    ths->size = 0;

    return ths;
}

void list_remove(struct list* ths)
{
    free(ths->arr);
    free(ths);
}

size_t list_capacity(struct list* ths)
{
    return ths->capacity;
}

size_t list_of_size(struct list* ths)
{
    return ths->of_size;
}

size_t list_size(struct list* ths)
{
    return ths->size;
}

bool list_empty(struct list* ths)
{
    return ths->size == 0;
}

bool list_full(struct list* ths)
{
    ths = ths;
    return false;
}

void __list_double(struct list* ths)
{
    ths->capacity *= 2;
    ths->arr = realloc(ths->arr, ths->capacity * ths->of_size);
}

void __list_half(struct list* ths)
{
    ths->capacity /= 2;
    ths->arr = realloc(ths->arr, ths->capacity * ths->of_size);
}

bool list_push(struct list* ths, void* value)
{
    if (ths->size == ths->capacity)
        __list_double(ths);

    memcpy((char*)ths->arr + ths->size++ * ths->of_size, value, ths->of_size);

    return true;
}

bool list_pop(struct list* ths, void* dest)
{
    if (ths->size == ths->capacity / 2)
        __list_half(ths);

    memcpy(dest, (char*)ths->arr + --ths->size * ths->of_size, ths->of_size);

    return true;
}

bool list_get(struct list* ths, void* dest, size_t index)
{
    memcpy(dest, (char*)ths->arr + index * ths->of_size, ths->of_size);

    return true;
}

void list_sort(struct list* ths, int (*comp)(const void*, const void*))
{
    qsort(ths->arr, ths->size, ths->of_size, comp);
}

void list_clear(struct list* ths)
{
    ths->arr = realloc(ths->arr, ths->of_size);
    ths->capacity = 1;
    ths->size = 0;
}

#endif