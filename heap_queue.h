#ifndef __HEAP_QUEUE_H
#define __HEAP_QUEUE_H

#include <stdio.h>
#include <stdbool.h>

#include "utils/memory.h"

/**
 * 배열을 기반으로 구현된 가변 크기 힙큐
 */
struct heap_queue
{
    /**
     * 실제 데이터를 저장할 공간에 대한 포인터
     */
    void* arr;
    /**
     * 힙큐의 한계 용량
     */
    size_t capacity;
    /**
     * 힙큐의 크기
     */
    size_t size;
    /**
     * 힙큐 단일 요소의 크기
     */
    size_t of_size;

    /**
     * 힙큐 연산 시에 사용되는 요소 비교 함수
     */
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
    ths->arr = realloc_s(ths->arr, ths->capacity * ths->of_size);
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
    ths->arr = realloc_s(ths->arr, ths->capacity * ths->of_size);
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

    ths->arr = realloc_s(ths->arr, correct_capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for stack in __heap_queue_capacity_correction().\n");
        abort();
    }

    ths->capacity = correct_capacity;
}

/**
 * *내부 함수
 * 
 * @brief 특정 인덱스에 대한 왼쪽 자식 요소의 인덱스 계산
 * @param index 왼쪽 자식 요소를 계산할 부모 요소의 인덱스
 * @return 왼쪽 자식 요소의 인덱스
 */
size_t __heap_queue_get_left_child(size_t index)
{
    return 2 * index + 1;
}

/**
 * *내부 함수
 * 
 * @brief 특정 인덱스에 대한 부모 요소의 인덱스 계산
 * @param index 부모 요소를 계산할 자식 요소의 인덱스
 * @return 부모 요소의 인덱스
 */
size_t __heap_queue_get_parent(size_t index)
{
    if (index == 0)
        return 0;
    return (index - 1) / 2;
}

/**
 * *내부 함수
 * 
 * @brief 특정 요소에 대해서 최상위 노드 방향으로 힙큐의 배열을 힙으로 변환
 * @param ths 대상 힙큐 포인터
 * @param index 힙으로 변환할 기준이 되는 요소의 인덱스
 */
void __heap_queue_reheap_up(struct heap_queue* ths, size_t index)
{
    size_t node = index;
    size_t parent = __heap_queue_get_parent(node);

    while (index > 0)
    {
        if (ths->comp((char*)ths->arr + node * ths->of_size, (char*)ths->arr + parent * ths->of_size) != -1)
            break;

        void* temp = malloc_s(ths->of_size);
        memcpy(temp, (char*)ths->arr + node * ths->of_size, ths->of_size);
        memcpy((char*)ths->arr + node * ths->of_size, (char*)ths->arr + parent * ths->of_size, ths->of_size);
        memcpy((char*)ths->arr + parent * ths->of_size, temp, ths->of_size);
        free_s(temp);

        node = parent;
        parent = __heap_queue_get_parent(node);
    }
}

/**
 * *내부 함수
 * 
 * @brief 특정 요소에 대해서 최하위 노드 방향으로 힙큐의 배열을 힙으로 변환
 * @param ths 대상 힙큐 포인터
 * @param index 힙으로 변환할 기준이 되는 요소의 인덱스
 */
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

        void* temp = malloc_s(ths->of_size);
        memcpy(temp, (char*)ths->arr + higher_priority * ths->of_size, ths->of_size);
        memcpy((char*)ths->arr + higher_priority * ths->of_size, (char*)ths->arr + node * ths->of_size, ths->of_size);
        memcpy((char*)ths->arr + node * ths->of_size, temp, ths->of_size);
        free_s(temp);

        node = higher_priority;
        child = __heap_queue_get_left_child(node);
    }
}

/**
 * *내부 함수
 * 
 * @brief 힙큐의 배열을 힙으로 변환
 * @param ths 대상 힙큐 포인터
 */
void __heap_queue_heapify(struct heap_queue* ths)
{
    if (ths->size == 0)
        return;

    size_t index = ths->size - 1;
    if (index % 2 == 1)
        index++;

    while (index > 0)
    {
        size_t parent = __heap_queue_get_parent(index);
        __heap_queue_reheap_down(ths, parent);
        index -= 2;
    }
}

/**
 * @brief 새로운 힙큐 생성
 * @param of_size 힙큐에 저장할 단일 요소의 크기
 * @param comp 힙큐 연산 시에 사용되는 요소 비교 함수
 * @return 동적으로 생성된 힙큐의 주소
 */
struct heap_queue* heap_queue_create(size_t of_size, int (*comp)(const void* p, const void* q))
{
    struct heap_queue* ths = (struct heap_queue*)malloc_s(sizeof(struct heap_queue));

    ths->arr = malloc_s(of_size);
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

/**
 * @brief 배열로부터 새로운 힙큐 생성
 * @param arr 힙큐로 생성할 배열의 포인터
 * @param size 힙큐로 생성할 배열의 길이
 * @param of_size 힙큐로 생성할 배열의 단일 요소의 크기
 * @return 동적으로 생성된 힙큐의 주소
 */
struct heap_queue* heap_queue_create_from_array(void* arr, size_t size, size_t of_size, int (*comp)(const void* p, const void* q))
{
    if (arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to initialize heap_queue since the original array is NULL.\n");
        abort();
    }
    else if (of_size == 0)
    {
        fprintf(stderr, "stderr: Size of a single element of heap_queue cannot be zero.");
        abort();
    }

    struct heap_queue* ths = (struct heap_queue*)malloc_s(sizeof(struct heap_queue));

    ths->arr = malloc_s(size * of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for heap_queue in heap_queue_create_from_array().\n");
        abort();
    }

    ths->capacity = size;
    ths->size = size;
    ths->of_size = of_size;
    ths->comp = comp;

    memcpy(ths->arr, arr, size * of_size);
    __heap_queue_capacity_correction(ths);
    __heap_queue_heapify(ths);

    return ths;
}

/**
 * @brief 기본값을 설정하여 새로운 힙큐 생성
 * @param value 힙큐의 기본값으로 설정할 값의 포인터
 * @param size 생성할 힙큐의 길이
 * @param of_size 힙큐의 기본값으로 설정할 값의 크기
 * @return 동적으로 생성된 힙큐의 주소
 */
struct heap_queue* heap_queue_create_from_value(void* value, size_t size, size_t of_size, int (*comp)(const void* p, const void* q))
{
    if (value == NULL)
    {
        fprintf(stderr, "stderr: Failed to initialize heap_queue since the original array is NULL.\n");
        abort();
    }
    else if (of_size == 0)
    {
        fprintf(stderr, "stderr: Size of a single element of heap_queue cannot be zero.");
        abort();
    }

    struct heap_queue* ths = (struct heap_queue*)malloc_s(sizeof(struct heap_queue));
    ths->arr = malloc_s(size * of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for heap_queue in heap_queue_create_from_array().\n");
        abort();
    }

    ths->capacity = size;
    ths->size = size;
    ths->of_size = of_size;
    ths->comp = comp;

    for (size_t i = 0; i < size; i++)
        memcpy((char*)ths->arr + i * of_size, value, of_size);
    __heap_queue_capacity_correction(ths);
    
    return ths;
}

/**
 * @brief 힙큐 삭제
 * @param ths 대상 힙큐 포인터
 */
void heap_queue_delete(struct heap_queue* ths)
{
    free_s(ths->arr);
    free_s(ths);
}

/**
 * @brief 힙큐가 비었는지 여부 반환
 * @param ths 대상 힙큐 포인터
 * @return 힙큐 빔 여부
 */
bool heap_queue_empty(struct heap_queue* ths)
{
    return ths->size == 0;
}

/**
 * @brief 힙큐에 새로운 요소 삽입
 * @param ths 대상 덱 포인터
 * @param value 삽입할 대상을 가리키는 포인터
 */
void heap_queue_push(struct heap_queue* ths, void* value)
{
    if (ths->size == ths->capacity)
        __heap_queue_double(ths);

    memcpy((char*)ths->arr + ths->size * ths->of_size, value, ths->of_size);
    __heap_queue_reheap_up(ths, ths->size);

    ths->size++;
}

/**
 * @brief 힙큐의 첫 요소를 삭제
 * @param ths 대상 힙큐 포인터
 */
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

/**
 * @brief 힙큐의 첫 요소를 dest에 복사
 * @param ths 대상 힙큐 포인터
 * @param dest 요소를 복사할 목적지
 */
void heap_queue_front(struct heap_queue* ths, void* dest)
{
    if (heap_queue_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to read from the front of the heap queue because the heap queue is empty.\n");
        abort();
    }

    memcpy(dest, ths->arr, ths->of_size);
}

/**
 * @brief 힙큐 초기화
 * @param ths 대상 힙큐 포인터
 */
void heap_queue_clear(struct heap_queue* ths)
{
    ths->arr = realloc_s(ths->arr, ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for stack in heap_queue_clear().\n");
        abort();
    }

    ths->capacity = 1;
    ths->size = 0;
}

#endif