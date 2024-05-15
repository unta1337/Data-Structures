#ifndef __UNDS_HASH_SET_H
#define __UNDS_HASH_SET_H

#include "unds_hash_map.h"

#ifdef UNDS_TRACK_MEM
#include "unds_memory.h"
#else
#include <stdlib.h>
#define unds_malloc malloc
#define unds_calloc calloc
#define unds_realloc realloc
#define unds_free free
#endif

bool __unds_true = true;
bool __unds_false = false;

struct unds_hash_set_t
{
    /**
     * 실제 데이터를 저장할 해시맵
     */
    unds_hash_map_t* map;
    /**
     * 해시셋의 한계 용량
     */
    size_t capacity;
    /**
     * 해시셋에 저장된 요소의 개수
     */
    size_t size;
    /**
     * 해시셋 키의 크기
     */
    size_t of_size;

    /**
     * 요소의 해싱에 사용되는 해시 함수
     */
    size_t (*hash)(const void* p);
    /**
     * 요소의 비교에 사용되는 비교 함수
     */
    int (*comp)(const void* p, const void* q);
};

typedef struct unds_hash_set_t unds_hash_set_t;

/**
 * *내부 함수.
 * 
 * @brief 해시셋의 속성을 해시맵과 동기화
 * @param ths 대상 해시셋 포인터
 */
void __unds_hash_set_update_variables(unds_hash_set_t* ths);

/**
 * *참고: hash는 키의 해싱에 사용되는 함수로서, 해시셋의 크기에 적절한 충분히 큰 값을 반환해야 한다.
 *        comp는 qsort 등에서 사용되는 비교 함수와 마찬가지로 다음의 반환값을 가져야 한다.
 *        comp(p, q)에서:
 *            p >  q -> -1
 *            p == q ->  0
 *            p <  q ->  1
 *
 * @brief 새로운 해시셋 생성
 * @param of_size 요소 자료형의 크기
 * @param hash 요소의 해싱에 사용되는 해시 함수
 * @param comp 요소의 비교에 사용되는 해시 함수
 * @return 동적으로 생성된 해시셋의 포인터
 */
unds_hash_set_t* unds_hash_set_create(size_t of_size, size_t (*hash)(const void *p), int (*comp)(const void* p, const void* q));

/**
 * @brief 해시셋 삭제
 * @param ths 대상 해시셋 포인터
 */
void unds_hash_set_delete(unds_hash_set_t* ths);

/**
 * @brief 해시셋에 새로운 요소 추가
 * @param ths 대상 해시셋 포인터
 * @param elem 추가할 요소의 포인터
 */
void unds_hash_set_push(unds_hash_set_t* ths, void* elem);

/**
 * @brief 해시셋에서 요소 삭제
 * @param ths 대상 해시셋 포인터
 * @param elem 삭제할 요소의 포인터
 */
void unds_hash_set_pop(unds_hash_set_t* ths, void* elem);

/**
 * @brief 요소의 존재 유무를 확인
 * @param ths 대상 해시셋 포인터
 * @param elem 존재 유무를 확인한 요소의 포인터
 * @return 해당 요소의 존재 유무
 */
bool unds_hash_set_has(unds_hash_set_t* ths, void* elem);

/**
 * @brief 해시셋 초기화
 * @param ths 대상 해시셋 포인터
 */
void unds_hash_set_clear(unds_hash_set_t* ths);

void __unds_hash_set_update_variables(unds_hash_set_t* ths)
{
    ths->capacity = ths->map->capacity;
    ths->size = ths->map->size;
    ths->of_size = ths->map->of_size_key;

    ths->hash = ths->map->hash;
    ths->comp = ths->map->comp;
}

unds_hash_set_t* unds_hash_set_create(size_t of_size, size_t (*hash)(const void *p), int (*comp)(const void* p, const void* q))
{
    unds_hash_set_t* ths = (unds_hash_set_t*)unds_malloc(sizeof(unds_hash_set_t));

    ths->map = unds_hash_map_create(of_size, sizeof(bool), hash, comp);

    __unds_hash_set_update_variables(ths);

    return ths;
}

void unds_hash_set_delete(unds_hash_set_t* ths)
{
    unds_hash_map_delete(ths->map);
    unds_free(ths);
}

void unds_hash_set_push(unds_hash_set_t* ths, void* elem)
{
    unds_hash_map_push(ths->map, elem, &__unds_true);

    __unds_hash_set_update_variables(ths);
}

void unds_hash_set_pop(unds_hash_set_t* ths, void* elem)
{
    unds_hash_map_pop(ths->map, elem);

    __unds_hash_set_update_variables(ths);
}

bool unds_hash_set_has(unds_hash_set_t* ths, void* elem)
{
    return unds_hash_map_has(ths->map, elem);
}

void unds_hash_set_clear(unds_hash_set_t* ths)
{
    unds_hash_map_clear(ths->map);

    __unds_hash_set_update_variables(ths);
}

#endif
