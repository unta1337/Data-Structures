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
    if (index == 0)
        return 0;
    return (index - 1) / 2;
}

void __heap_queue_reheap_up(struct heap_queue* ths, size_t index)
{
    size_t node = index;
    size_t parent = __heap_queue_get_parent(node);

    while (index > 0)
    {
        if (ths->comp((char*)ths->arr + node * ths->of_size, (char*)ths->arr + parent * ths->of_size) != -1)
            break;

        void* temp = malloc(ths->of_size);
        memcpy(temp, (char*)ths->arr + node * ths->of_size, ths->of_size);
        memcpy((char*)ths->arr + node * ths->of_size, (char*)ths->arr + parent * ths->of_size, ths->of_size);
        memcpy((char*)ths->arr + parent * ths->of_size, temp, ths->of_size);
        free(temp);

        node = parent;
        parent = __heap_queue_get_parent(node);
    }
}

void __heap_queue_reheap_down(struct heap_queue* ths, size_t index)
{
    size_t node = index;
    size_t child = __heap_queue_get_left_child(index);

    while (child < ths->size)
    {
        int comp_result = -1;
        if (child + 1 < ths->size)
            comp_result = ths->comp((char*)ths->arr + child * ths->of_size, (char*)ths->arr + (child + 1) * ths->of_size);

        int higher_priority = comp_result == -1 ? child : child + 1;

        if (ths->comp((char*)ths->arr + higher_priority * ths->of_size, (char*)ths->arr + node * ths->of_size) != -1)
            break;

        void* temp = malloc(ths->of_size);
        memcpy(temp, (char*)ths->arr + higher_priority * ths->of_size, ths->of_size);
        memcpy((char*)ths->arr + higher_priority * ths->of_size, (char*)ths->arr + node * ths->of_size, ths->of_size);
        memcpy((char*)ths->arr + node * ths->of_size, temp, ths->of_size);
        free(temp);

        node = higher_priority;
        child = __heap_queue_get_left_child(node);
    }
}

void __heap_queue_heapify(struct heap_queue* ths)
{
    size_t index = ths->size - 1;
    if (index % 2 == 0)
        index--;

    while (index > 0)
    {
        size_t parent = __heap_queue_get_parent(index);
        __heap_queue_reheap_down(ths, parent);
        index -= 2;
    }
}

struct heap_queue* heap_queue_create(size_t of_size, int (*comp)(const void* p, const void* q))
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

bool heap_queue_empty(struct heap_queue* ths)
{
    return ths->size == 0;
}

void heap_queue_push(struct heap_queue* ths, void* value)
{
    if (ths->size == ths->capacity)
        __heap_queue_double(ths);

    memcpy((char*)ths->arr + ths->size * ths->of_size, value, ths->of_size);
    __heap_queue_reheap_up(ths, ths->size);

    ths->size++;
}

void heap_queue_pop(struct heap_queue* ths)
{
    if (heap_queue_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to pop an element from heap queue because the heap queue is empty.\n");
        abort();
    }

    ths->size--;

    memcpy(ths->arr, (char*)ths->arr + ths->size * ths->of_size, ths->of_size);
    __heap_queue_reheap_down(ths, 0);

    if (ths->size == ths->capacity / 2)
        __heap_queue_half(ths);
}

void heap_queue_front(struct heap_queue* ths, void* dest)
{
    if (heap_queue_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to read from the front of the heap queue because the heap queue is empty.\n");
        abort();
    }

    memcpy(dest, ths->arr, ths->of_size);
}

void heap_queue_clear(struct heap_queue* ths)
{
    ths->arr = realloc(ths->arr, ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for stack in heap_queue_clear().\n");
        abort();
    }

    ths->capacity = 1;
    ths->size = 0;
}

#endif