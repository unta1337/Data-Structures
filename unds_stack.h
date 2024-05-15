#ifndef __UNDS_STACK_H
#define __UNDS_STACK_H

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
 * 배열을 기반으로 구현된 가변 크기 스택
 */
struct unds_stack_t
{
    /**
     * 실제 데이터를 저장할 공간에 대한 포인터
     */
    void* arr;
    /**
     * 스택의 한계 용량
     */
    size_t capacity;
    /**
     * 스택의 크기
     */
    size_t size;
    /**
     * 스택 단일 요소의 크기
     */
    size_t of_size;
};

typedef struct unds_stack_t unds_stack_t;

/**
 * *내부 함수
 *
 * @brief 스택 크기 2배 증가
 * @param ths 대상 스택 포인터
 */
void __unds_stack_double(unds_stack_t* ths)
{
    ths->capacity *= 2;
    ths->arr = unds_realloc(ths->arr, ths->capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for stack in __stack_double().\n");
        abort();
    }
}

/**
 * *내부 함수
 *
 * @brief 스택 크기 2배 감소
 * @param ths 대상 스택 포인터
 */
void __unds_stack_half(unds_stack_t* ths)
{
    if (ths->size == 0)
        return;

    ths->capacity /= 2;
    ths->arr = unds_realloc(ths->arr, ths->capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for stack in __stack_half().\n");
        abort();
    }
}

/**
 * *내부 함수
 *
 * @brief 스택의 크기를 2의 제곱수로 교정
 * @param ths 대상 스택 포인터
 */
void __unds_stack_capacity_correction(unds_stack_t* ths)
{
    size_t correct_capacity = 1;

    while (correct_capacity <= ths->size)
        correct_capacity *= 2;

    ths->arr = unds_realloc(ths->arr, correct_capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for stack in __stack_capacity_correction().\n");
        abort();
    }

    ths->capacity = correct_capacity;
}

/**
 * @brief 새로운 스택 생성
 * @param of_size 스택에 저장할 단일 요소의 크기
 * @return 동적으로 생성된 스택의 주소
 */
unds_stack_t* unds_stack_create(size_t of_size)
{
    unds_stack_t* ths = (unds_stack_t*)unds_malloc(sizeof(unds_stack_t));

    ths->arr = unds_malloc(of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for stack in stack_create().\n");
        abort();
    }

    ths->capacity = 1;
    ths->of_size = of_size;
    ths->size = 0;

    return ths;
}

/**
 * @brief 배열로부터 새로운 스택 생성
 * @param arr 스택으로 생성할 배열의 포인터
 * @param size 스택으로 생성할 배열의 길이
 * @param of_size 스택으로 생성할 배열의 단일 요소의 크기
 * @return 동적으로 생성된 스택의 주소
 */
unds_stack_t* unds_stack_create_from_array(void* arr, size_t size, size_t of_size)
{
    if (arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to initialize stack since the original array is NULL.\n");
        abort();
    }
    else if (of_size == 0)
    {
        fprintf(stderr, "stderr: Size of a single element of stack cannot be zero.");
        abort();
    }

    unds_stack_t* ths = (unds_stack_t*)unds_malloc(sizeof(unds_stack_t));

    ths->arr = unds_malloc(size * of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for stack in stack_create_from_array().\n");
        abort();
    }

    ths->capacity = size;
    ths->size = size;
    ths->of_size = of_size;

    memcpy(ths->arr, arr, size * of_size);
    __unds_stack_capacity_correction(ths);

    return ths;
}

/**
 * @brief 기본값을 설정하여 새로운 스택 생성
 * @param value 스택의 기본값으로 설정할 값의 포인터
 * @param size 생성할 스택의 길이
 * @param of_size 스택의 기본값으로 설정할 값의 크기
 * @return 동적으로 생성된 스택의 주소
 */
unds_stack_t* unds_stack_create_from_value(void* value, size_t size, size_t of_size)
{
    if (value == NULL)
    {
        fprintf(stderr, "stderr: Failed to initialize stack since the original array is NULL.\n");
        abort();
    }
    else if (of_size == 0)
    {
        fprintf(stderr, "stderr: Size of a single element of stack cannot be zero.");
        abort();
    }

    unds_stack_t* ths = (unds_stack_t*)unds_malloc(sizeof(unds_stack_t));
    ths->arr = unds_malloc(size * of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for stack in stack_create_from_array().\n");
        abort();
    }

    ths->capacity = size;
    ths->size = size;
    ths->of_size = of_size;

    for (size_t i = 0; i < size; i++)
        memcpy((char*)ths->arr + i * of_size, value, of_size);
    __unds_stack_capacity_correction(ths);
    
    return ths;
}

/**
 * @brief 스택 삭제
 * @param ths 대상 스택 포인터
 */
void unds_stack_delete(unds_stack_t* ths)
{
    unds_free(ths->arr);
    unds_free(ths);
}

/**
 * @brief 스택이 비었는지 여부 반환
 * @param ths 대상 스택 포인터
 * @return 스택 빔 여부
 */
bool unds_stack_empty(unds_stack_t* ths)
{
    return ths->size == 0;
}

/**
 * @brief 스택에 새로운 요소 삽입
 * @param ths 대상 스택 포인터
 * @param value 삽입할 대상을 가리키는 포인터
 */
void unds_stack_push(unds_stack_t* ths, void* value)
{
    if (ths->size == ths->capacity)
        __unds_stack_double(ths);

    memcpy((char*)ths->arr + ths->size++ * ths->of_size, value, ths->of_size);
}

/**
 * @brief 스택의 마지막 요소를 삭제
 * @param ths 대상 스택 포인터
 */
void unds_stack_pop(unds_stack_t* ths)
{
    if (unds_stack_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to pop an element from stack because the stack is empty.\n");
        abort();
    }

    ths->size--;

    if (ths->size == ths->capacity / 2)
        __unds_stack_half(ths);
}

/**
 * @brief 스택의 마지막 요소를 dest에 복사
 * @param ths 대상 스택 포인터
 * @param dest 요소를 복사할 목적지
 */
void unds_stack_top(unds_stack_t* ths, void* dest)
{
    if (unds_stack_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to read from the top of the stack because the stack is empty.\n");
        abort();
    }

    memcpy(dest, (char*)ths->arr + (ths->size - 1) * ths->of_size, ths->of_size);
}

/**
 * @brief 스택 초기화
 * @param ths 대상 스택 포인터
 */
void unds_stack_clear(unds_stack_t* ths)
{
    ths->arr = unds_realloc(ths->arr, ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for stack in stack_clear().\n");
        abort();
    }

    ths->capacity = 1;
    ths->size = 0;
}

#endif
