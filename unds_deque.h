#ifndef __UNDS_DEQUE_H
#define __UNDS_DEQUE_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifdef UNDS_TRACK_MEM
#include "unds_memory.h"
#else
#include <stdlib.h>
#define unds_malloc malloc
#define unds_calloc calloc
#define unds_realloc realloc
#define unds_free free
#endif

/**
 * 배열을 기반으로 구현된 가변 크기 덱
 */
struct unds_deque_t
{
    /**
     * 실제 데이터를 저장할 공간에 대한 포인터
     */
    void* arr;
    /**
     * 덱의 한계 용량
     */
    size_t capacity;
    /**
     * 덱의 크기
     */
    size_t size;
    /**
     * 덱 단일 요소의 크기
     */
    size_t of_size;
    /**
     * 덱의 앞부분을 나타내는 인덱스
     */
    size_t head;
    /**
     * 덱의 뒷부분을 나타내는 인덱스
     */
    size_t tail;
};

typedef struct unds_deque_t unds_deque_t;

/**
 * *내부 함수
 * 
 * @brief 덱 크기 2배 증가
 * @param ths 대상 덱 포인터
 */
void __unds_deque_double(unds_deque_t* ths)
{
    ths->capacity *= 2;
    ths->arr = unds_realloc(ths->arr, ths->capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for deque in __unds_deque_double().\n");
        abort();
    }

    if (ths->head > ths->tail)
    {
        size_t head_to_end = ths->capacity / 2 - ths->head;

        memmove((char*)ths->arr + (ths->capacity - head_to_end) * ths->of_size, (char*)ths->arr + ths->head * ths->of_size, head_to_end * ths->of_size);

        ths->head = ths->capacity - head_to_end;
    }
}

/**
 * *내부 함수
 * 
 * @brief 덱 크기 2배 감소
 * @param ths 대상 덱 포인터
 */
void __unds_deque_half(unds_deque_t* ths)
{
    if (ths->size == 0)
        return;

    if (ths->head < ths->tail)
        memmove(ths->arr, (char*)ths->arr + ths->head * ths->of_size, ths->size * ths->of_size);
    else
    {
        size_t head_to_end = ths->capacity - ths->head;
        size_t begin_to_tail = ths->tail;

        memmove((char*)ths->arr + head_to_end * ths->of_size, ths->arr, begin_to_tail * ths->of_size);
        memmove(ths->arr, (char*)ths->arr + ths->head * ths->of_size, head_to_end * ths->of_size);
    }

    ths->capacity /= 2;
    ths->arr = unds_realloc(ths->arr, ths->capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for deque in __unds_deque_half().\n");
        abort();
    }

    ths->head = 0;
    ths->tail = ths->capacity - 1;
}

/**
 * *내부 함수
 *
 * @brief 덱의 크기를 2의 제곱수로 교정
 * @param ths 대상 덱 포인터
 */
void __unds_deque_capacity_correction(unds_deque_t* ths)
{
    size_t correct_capacity = 1;

    while (correct_capacity <= ths->size + 1)
        correct_capacity *= 2;

    ths->arr = unds_realloc(ths->arr, correct_capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for deque in __unds_deque_capacity_correction().\n");
        abort();
    }

    ths->capacity = correct_capacity;
}

/**
 * @brief 새로운 덱 생성
 * @param of_size 덱에 저장할 단일 요소의 크기
 * @return 동적으로 생성된 덱의 주소
 */
unds_deque_t* unds_deque_create(size_t of_size)
{
    unds_deque_t* ths = (unds_deque_t*)unds_malloc(sizeof(unds_deque_t));

    ths->arr = unds_malloc(of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for deque in unds_deque_create().\n");
        abort();
    }

    ths->capacity = 1;
    ths->size = 0;
    ths->of_size = of_size;
    ths->head = 0;
    ths->tail = 0;

    return ths;
}

/**
 * @brief 배열로부터 새로운 덱 생성
 * @param arr 덱으로 생성할 배열의 포인터
 * @param size 덱으로 생성할 배열의 길이
 * @param of_size 덱으로 생성할 배열의 단일 요소의 크기
 * @return 동적으로 생성된 덱의 주소
 */
unds_deque_t* unds_deque_create_from_array(void* arr, size_t size, size_t of_size)
{
    if (arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to initialize deque since the original array is NULL.\n");
        abort();
    }
    else if (of_size == 0)
    {
        fprintf(stderr, "stderr: Size of a single element of deque cannot be zero.");
        abort();
    }

    unds_deque_t* ths = (unds_deque_t*)unds_malloc(sizeof(unds_deque_t));

    ths->arr = unds_malloc(size * of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for deque in unds_deque_create_from_array().\n");
        abort();
    }

    ths->capacity = size;
    ths->size = size;
    ths->of_size = of_size;
    ths->head = 0;
    ths->tail = size;

    memcpy(ths->arr, arr, size * of_size);
    __unds_deque_capacity_correction(ths);

    return ths;
}

/**
 * @brief 기본값을 설정하여 새로운 덱 생성
 * @param value 덱의 기본값으로 설정할 값의 포인터
 * @param size 생성할 덱의 길이
 * @param of_size 덱의 기본값으로 설정할 값의 크기
 * @return 동적으로 생성된 덱의 주소
 */
unds_deque_t* deque_create_from_value(void* value, size_t size, size_t of_size)
{
    if (value == NULL)
    {
        fprintf(stderr, "stderr: Failed to initialize deque since the original array is NULL.\n");
        abort();
    }
    else if (of_size == 0)
    {
        fprintf(stderr, "stderr: Size of a single element of deque cannot be zero.");
        abort();
    }

    unds_deque_t* ths = (unds_deque_t*)unds_malloc(sizeof(unds_deque_t));
    ths->arr = unds_malloc(size * of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for deque in unds_deque_create_from_array().\n");
        abort();
    }

    ths->capacity = size;
    ths->size = size;
    ths->of_size = of_size;
    ths->head = 0;
    ths->tail = size;

    for (size_t i = 0; i < size; i++)
        memcpy((char*)ths->arr + i * of_size, value, of_size);
    __unds_deque_capacity_correction(ths);
    
    return ths;
}

/**
 * @brief 덱 삭제
 * @param ths 대상 덱 포인터
 */
void deque_delete(unds_deque_t* ths)
{
    unds_free(ths->arr);
    unds_free(ths);
}

/**
 * @brief 덱이 비었는지 여부 반환
 * @param ths 대상 덱 포인터
 * @return 덱 빔 여부
 */
bool deque_empty(unds_deque_t* ths)
{
    return ths->size == 0;
}

/**
 * @brief 덱에 앞부분에 새로운 요소 삽입
 * @param ths 대상 덱 포인터
 * @param value 삽입할 대상을 가리키는 포인터
 */
void deque_push_front(unds_deque_t* ths, void* value)
{
    if (ths->size + 1 == ths->capacity)
        __unds_deque_double(ths);

    ths->head += ths->capacity - 1;
    ths->head %= ths->capacity;

    memcpy((char*)ths->arr + ths->head * ths->of_size, value, ths->of_size);

    ths->size++;
}

/**
 * @brief 덱에 뒷부분에 새로운 요소 삽입
 * @param ths 대상 덱 포인터
 * @param value 삽입할 대상을 가리키는 포인터
 */
void deque_push_back(unds_deque_t* ths, void* value)
{
    if (ths->size + 1 == ths->capacity)
        __unds_deque_double(ths);

    memcpy((char*)ths->arr + ths->tail * ths->of_size, value, ths->of_size);

    ths->tail++;
    ths->tail %= ths->capacity;

    ths->size++;
}

/**
 * @brief 덱의 마지막 요소를 삭제
 * @param ths 대상 덱 포인터
 */
void deque_pop_front(unds_deque_t* ths)
{
    if (deque_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to pop an element from deque because the deque is empty.\n");
        abort();
    }

    ths->head++;
    ths->head %= ths->capacity;

    ths->size--;

    if (ths->size + 1 == ths->capacity / 2)
        __unds_deque_half(ths);
}

/**
 * @brief 덱의 마지막 요소를 삭제
 * @param ths 대상 덱 포인터
 */
void deque_pop_back(unds_deque_t* ths)
{
    if (deque_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to pop an element from deque because the deque is empty.\n");
        abort();
    }

    ths->tail += ths->capacity - 1;
    ths->tail %= ths->capacity;

    ths->size--;

    if (ths->size + 1 == ths->capacity / 2)
        __unds_deque_half(ths);
}

/**
 * @brief 덱의 첫 요소를 dest에 복사
 * @param ths 대상 덱 포인터
 * @param dest 요소를 복사할 목적지
 */
void deque_front(unds_deque_t* ths, void* dest)
{
    if (deque_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to read from the front of the deque because the deque is empty.\n");
        abort();
    }

    memcpy(dest, (char*)ths->arr + ths->head * ths->of_size, ths->of_size);
}

/**
 * @brief 덱의 마지막 요소를 dest에 복사
 * @param ths 대상 덱 포인터
 * @param dest 요소를 복사할 목적지
 */
void deque_back(unds_deque_t* ths, void* dest)
{
    if (deque_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to read from the back of the deque because the deque is empty.\n");
        abort();
    }

    memcpy(dest, (char*)ths->arr + (ths->tail + ths->capacity - 1) % ths->capacity * ths->of_size, ths->of_size);
}



/**
 * @brief 덱 초기화
 * @param ths 대상 덱 포인터
 */
void deque_clear(unds_deque_t* ths)
{
    ths->arr = unds_realloc(ths->arr, ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for stack in deque_clear().\n");
        abort();
    }

    ths->capacity = 1;
    ths->size = 0;
    ths->head = 0;
    ths->tail = 0;
}

#endif
