#ifndef __HEAP_QUEUE_H
#define __HEAP_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct heap_queue
{
    void* arr;
    size_t capacity;
    size_t size;
    size_t of_size;

    int (*comp)(const void* p, const void* q);
};

/**
 * *내부 함수
 *
 * @brief 힙큐 크기 2배 증가
 * @param ths 대상 힙큐 포인터
 */
void __heap_queue_double(struct heap_queue* ths)
{
    ths->capacity *= 2;
    ths->arr = realloc(ths->arr, ths->capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for heap_queue in __heap_queue_double().\n");
        abort();
    }
}

/**
 * *내부 함수
 *
 * @brief 힙큐 크기 2배 감소
 * @param ths 대상 힙큐 포인터
 */
void __heap_queue_half(struct heap_queue* ths)
{
    if (ths->size == 0)
        return;

    ths->capacity /= 2;
    ths->arr = realloc(ths->arr, ths->capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for heap_queue in __heap_queue_half().\n");
        abort();
    }
}

/**
 * *내부 함수
 *
 * @brief 힙큐의 크기를 2의 제곱수로 교정
 * @param ths 대상 힙큐 포인터
 */
void __heap_queue_capacity_correction(struct heap_queue* ths)
{
    size_t correct_capacity = 1;

    while (correct_capacity <= ths->size)
        correct_capacity *= 2;

    ths->arr = realloc(ths->arr, correct_capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for stack in __heap_queue_capacity_correction().\n");
        abort();
    }

    ths->capacity = correct_capacity;
}

size_t __heap_queue_get_left_child(size_t index)
{
    return 2 * index + 1;
}

size_t __heap_queue_get_parent(size_t index)
{
    return (index - 1) / 2;
}

void __heap_queue_reheap_up(struct heap_queue* ths, size_t index)
{
    size_t node = index;
    size_t parent = __heap_queue_get_parent(node);

    while (parent >= 0)
    {
        if (ths->comp((char*)ths->arr + node * ths->of_size, (char*)ths->arr + parent * ths->of_size) == -1)
        {
            void* temp = malloc(of_size);
            memcpy((char*)ths->arr + node * ths->of_size, temp, ths->of_size);
            memcpy((char*)ths->arr + node * ths->of_size, (char*)ths->arr + parent * ths->of_size, ths->of_size);
            memcpy((char*)ths->arr + parent * ths->of_size, temp, ths->of_size);
            free(temp);

            node = parent;
            parent = __heap_queue_get_parent(node);

            continue;
        }

        break;
    }
}

void __heap_queue_reheap_down(struct heap_queue* ths, size_t index)
{
}

struct heap_queue* heap_queue_create(size_t of_size int (*comp)(const void* p, const void* q))
{
    struct heap_queue* ths = (struct heap_queue*)malloc(sizeof(struct heap_queue));

    ths->arr = malloc(of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for heap queue in heap_queue_create()\n");
        abort();
    }

    ths->capacity = 1;
    ths->size = 0;
    ths->of_size = of_size;
    ths->comp = comp;

    return ths;
}

void heap_queue_delete(struct heap_queue* ths)
{
    free(ths->arr);
    free(ths);
}

#endif
