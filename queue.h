#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct queue
{
    void* arr;
    size_t capacity;
    size_t size;

    size_t head;
    size_t tail;
};

struct queue* queue_create(size_t capacity, size_t size)
{
    struct queue* ths = (struct queue*)malloc(sizeof(struct queue));

    ths->arr = malloc((capacity + 1) * size);
    ths->capacity = capacity;
    ths->size = size;

    ths->head = 0;
    ths->tail = 0;

    return ths;
}

void queue_remove(struct queue* ths)
{
    free(ths->arr);
    free(ths);
}

size_t queue_capacity(struct queue* ths)
{
    return ths->capacity;
}

size_t queue_of_size(struct queue* ths)
{
    return ths->size;
}

size_t queue_size(struct queue* ths)
{
    size_t begin = ths->head;
    size_t end = ths->head <= ths->tail ? ths->tail : ths->tail + ths->capacity + 1;

    return end - begin;
}

bool queue_empty(struct queue* ths)
{
    return queue_size(ths) == 0;
}

bool queue_full(struct queue* ths)
{
    return queue_size(ths) == ths->capacity;
}

bool queue_push(struct queue* ths, void* value)
{
    if (queue_full(ths))
        return false;

    memcpy((char*)ths->arr + ths->tail++ * ths->size, value, ths->size);
    ths->tail %= ths->capacity + 1;

    return true;
}

void queue_pop(struct queue* ths, void* dest)
{
    if (queue_empty(ths))
        return;

    memcpy(dest, (char*)ths->arr + ths->head++ * ths->size, ths->size);
    ths->head %= ths->capacity + 1;
}

void queue_clear(struct queue* ths)
{
    ths->head = 0;
    ths->tail = 0;
}

#endif