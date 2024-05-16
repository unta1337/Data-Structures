/**
 * 메모리 조작 관련 헤더 및 동적할당된 메모리의 크기를 추적하기 위한 유틸리티 모음
 */

// Ref: https://stackoverflow.com/questions/8259817/how-to-track-malloc-and-free

#ifndef __UNDS_MEMORY_H
#define __UNDS_MEMORY_H

#include <stdlib.h>
#include <string.h>

/**
 * 현재까지 동적할당된 메모리의 총량
 */
extern size_t unds_used_malloc;

/**
 * @brief 메모리 사용량을 추적하는 malloc
 * @param size 할당 받을 메모리의 크기
 * @return 동적할당된 포인터
 */
void* unds_malloc(size_t size);

/**
 * @brief 메모리 사용량을 추적하는 calloc
 * @param n 할당 받을 요소의 개수
 * @param of_size 할당 받을 단일 요소의 크기
 * @return 동적할당된 포인터
 */
void* unds_calloc(size_t n, size_t of_size);

/**
 * @brief 메모리 사용량을 추적하는 realloc
 * @param ptr 재할당 받을 포인터
 * @param size 할당 받을 메모리의 크기
 * @return 동적할당된 포인터
 */
void* unds_realloc(void* ptr, size_t size);

#ifdef UNDS_MEMORY_IMPLEMENTATION

size_t unds_used_malloc = 0;

void* unds_malloc(size_t size)
{
    void* buffer = malloc(size + sizeof(size_t));
    if (buffer == NULL)
        return NULL;

    unds_used_malloc += size;

    size_t* size_box = (size_t*)buffer;
    *size_box = size;

    return (char*)buffer + sizeof(size_t);
}

void* unds_calloc(size_t n, size_t of_size)
{
    void* buffer = malloc(n * of_size + sizeof(size_t));
    if (buffer == NULL)
        return NULL;

    memset(buffer, 0, n * of_size + sizeof(size_t));

    unds_used_malloc += n * of_size;

    size_t* size_box = (size_t*)buffer;
    *size_box = n * of_size;

    return (char*)buffer + sizeof(size_t);
}

void* unds_realloc(void* ptr, size_t size)
{
    void* buffer = (char*)ptr - sizeof(size_t);
    size_t* size_box = (size_t*)buffer;

    unds_used_malloc -= *size_box;

    buffer = realloc(buffer, size + sizeof(size_t));
    if (buffer == NULL)
        return NULL;

    size_box = (size_t*)buffer;

    unds_used_malloc += size;
    *size_box = size;

    return (char*)buffer + sizeof(size_t);
}

void unds_free(void* ptr)
{
    if (ptr == NULL)
        return;

    void* buffer = (char*)ptr - sizeof(size_t);
    size_t* size_box = (size_t*)buffer;

    unds_used_malloc -= *size_box;

    free(buffer);
}

#endif

#endif
