#ifndef __UNDS_PAIR_H
#define __UNDS_PAIR_H

#include <stdio.h>
#include <string.h>

#ifdef UNDS_TRACK_MEM
#include "utils/memory.h"
#else
#include <stdlib.h>
#define unds_malloc malloc
#define unds_calloc calloc
#define unds_realloc realloc
#define unds_free free
#endif

/**
 * 서로 같거나 다른 자료형의 두 변수를 유지하는 페어
 */
struct unds_pair_t
{
    /**
     * 페어의 첫 번째 요소
     */
    void* first;
    /**
     * 페어의 두 번째 요소
     */
    void* second;

    /**
     * 페어의 첫 번째 요소의 크기
     */
    size_t of_size_first;
    /**
     * 페어의 두 번째 요소의 크기
     */
    size_t of_size_second;
};

typedef struct unds_pair_t unds_pair_t;

/**
 * @brief 페어 생성
 * @param of_size_first 첫 번째 요소의 크기
 * @param of_size_first 두 번째 요소의 크기
 * @return 동적할당된 페어의 포인터
 */
unds_pair_t* unds_pair_create(size_t of_size_first, size_t of_size_second)
{
    if (of_size_first == 0 || of_size_second == 0)
    {
        fprintf(stderr, "stderr: Size of a single element of pair cannot be zero.\n");
        abort();
    }

    unds_pair_t* ths = (unds_pair_t*)unds_malloc(sizeof(unds_pair_t));

    ths->first = unds_malloc(of_size_first);
    ths->second = unds_malloc(of_size_second);
    if (ths->first == NULL || ths->second == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for pair in pair_create().\n");
        abort();
    }

    ths->of_size_first = of_size_first;
    ths->of_size_second = of_size_second;

    return ths;
}

/**
 * @brief 초기값을 설정하여 페어 생성
 * @param first 첫 번재 요소
 * @param of_size_first 첫 번째 요소의 크기
 * @param second 두 번재 요소
 * @param of_size_first 두 번째 요소의 크기
 * @return 동적할당된 페어의 포인터
 */
unds_pair_t* unds_pair_make(void* first, size_t of_size_first, void* second, size_t of_size_second)
{
    if (first == NULL || second == NULL)
    {
        fprintf(stderr, "stderr: Pair element cannot be NULL.\n");
        abort();
    }

    unds_pair_t* ths = unds_pair_create(of_size_first, of_size_second);

    memcpy(ths->first, first, ths->of_size_first);
    memcpy(ths->second, second, ths->of_size_second);

    return ths;
}

/**
 * @brief 페어 삭제
 * @param 삭제할 페어의 포인터
 */
void unds_pair_delete(unds_pair_t* ths)
{
    unds_free(ths->first);
    unds_free(ths->second);
    unds_free(ths);
}

/**
 * @brief 페어의 첫 번재 요소를 dest에 복사
 * @param ths 대상 페어 포인터
 * @param dest 복사할 목적지
 */
void unds_pair_get_first(unds_pair_t* ths, void* dest)
{
    memcpy(dest, ths->first, ths->of_size_first);
}

/**
 * @brief 페어의 두 번재 요소를 dest에 복사
 * @param ths 대상 페어 포인터
 * @param dest 복사할 목적지
 */
void unds_pair_get_second(unds_pair_t* ths, void* dest)
{
    memcpy(dest, ths->second, ths->of_size_second);
}

/**
 * @brief 페어의 첫 번재 요소를 value로 설정
 * @param ths 대상 페어 포인터
 * @param value 설정할 값
 */
void unds_pair_set_first(unds_pair_t* ths, void* value)
{
    memcpy(ths->first, value, ths->of_size_first);
}

/**
 * @brief 페어의 두 번재 요소를 value로 설정
 * @param ths 대상 페어 포인터
 * @param value 설정할 값
 */
void unds_pair_set_second(unds_pair_t* ths, void* value)
{
    memcpy(ths->second, value, ths->of_size_second);
}

#endif
