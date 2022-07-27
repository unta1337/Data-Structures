#ifndef _STACK_H
#define _STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct stack
{
    void* arr;
    size_t capacity;
    size_t size;

    size_t top;
};

struct stack* stack_create(size_t capacity, size_t size)
{
    struct stack* ths = (struct stack*)malloc(sizeof(struct stack));

    ths->arr = malloc(capacity * size);
    ths->capacity = capacity;
    ths->size = size;

    ths->top = 0;

    return ths;
}

void stack_remove(struct stack* ths)
{
    free(ths->arr);
    free(ths);
}

size_t stack_capacity(struct stack* ths)
{
    return ths->capacity;
}

size_t stack_of_size(struct stack* ths)
{
    return ths->size;
}

size_t stack_size(struct stack* ths)
{
    return ths->top;
}

bool stack_empty(struct stack* ths)
{
    return stack_size(ths) == 0;
}

bool stack_full(struct stack* ths)
{
    return stack_size(ths) == ths->capacity;
}

bool stack_push(struct stack* ths, void* value)
{
    if (stack_full(ths))
        return false;
    
    memcpy((char*)ths->arr + ths->top++ * ths->size, value, ths->size);

    return true;
}

void stack_pop(struct stack* ths, void* dest)
{
    if (stack_empty(ths))
        return;

    memcpy(dest, (char*)ths->arr + --ths->top * ths->size, ths->size);
}

#endif