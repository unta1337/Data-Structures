#ifndef __HASH_SET_H
#define __HASH_SET_H

#include "hash_map.h"

bool __true = true;
bool __false = false;

struct hash_set
{
    /**
     * 실제 데이터를 저장할 해시맵
     */
    struct hash_map* map;
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

/**
 * *내부 함수.
 * 
 * @brief 해시셋의 속성을 해시맵과 동기화
 * @param ths 대상 해시셋 포인터
 */
void __hash_set_update_variables(struct hash_set* ths);

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
struct hash_set* hash_set_create(size_t of_size, size_t (*hash)(const void *p), int (*comp)(const void* p, const void* q));

/**
 * @brief 해시셋 삭제
 * @param ths 대상 해시셋 포인터
 */
void hash_set_delete(struct hash_set* ths);

/**
 * @brief 해시셋에 새로운 요소 추가
 * @param ths 대상 해시셋 포인터
 * @param elem 추가할 요소의 포인터
 */
void hash_set_push(struct hash_set* ths, void* elem);

/**
 * @brief 해시셋에서 요소 삭제
 * @param ths 대상 해시셋 포인터
 * @param elem 삭제할 요소의 포인터
 */
void hash_set_pop(struct hash_set* ths, void* elem);

/**
 * @brief 요소의 존재 유무를 확인
 * @param ths 대상 해시셋 포인터
 * @param elem 존재 유무를 확인한 요소의 포인터
 * @return 해당 요소의 존재 유무
 */
bool hash_set_has(struct hash_set* ths, void* elem);

/**
 * @brief 해시셋 초기화
 * @param ths 대상 해시셋 포인터
 */
void hash_set_clear(struct hash_set* ths);

void __hash_set_update_variables(struct hash_set* ths)
{
    ths->capacity = ths->map->capacity;
    ths->size = ths->map->size;
    ths->of_size = ths->map->of_size_key;

    ths->hash = ths->map->hash;
    ths->comp = ths->map->comp;
}

struct hash_set* hash_set_create(size_t of_size, size_t (*hash)(const void *p), int (*comp)(const void* p, const void* q))
{
    struct hash_set* ths = (struct hash_set*)malloc_s(sizeof(struct hash_set));

    ths->map = hash_map_create(of_size, sizeof(bool), hash, comp);

    __hash_set_update_variables(ths);

    return ths;
}

void hash_set_delete(struct hash_set* ths)
{
    hash_map_delete(ths->map);
    free_s(ths);
}

void hash_set_push(struct hash_set* ths, void* elem)
{
    hash_map_push(ths->map, elem, &__true);

    __hash_set_update_variables(ths);
}

void hash_set_pop(struct hash_set* ths, void* elem)
{
    hash_map_pop(ths->map, elem);

    __hash_set_update_variables(ths);
}

bool hash_set_has(struct hash_set* ths, void* elem)
{
    return hash_map_has(ths->map, elem);
}

void hash_set_clear(struct hash_set* ths)
{
    hash_map_clear(ths->map);

    __hash_set_update_variables(ths);
}

#endif