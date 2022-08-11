#ifndef __HASH_MAP_H
#define __HASH_MAP_H

#include <stdio.h>
#include <stdbool.h>

#include "utils/memory.h"

#include "list.h"
#include "pair.h"

/**
 * 리스트와 페어를 기반으로 구현된 가변 크기 해시맵
 */
struct hash_map
{
    /**
     * 실제 데이터를 저장하기 위한 리스트
     */
    struct list** arr;
    /**
     * 해시맵의 한계 용량
     */
    size_t capacity;
    /**
     * 해시맵에 저장된 요소의 개수
     */
    size_t size;
    /**
     * 해시맵 키의 크기
     */
    size_t of_size_key;
    /**
     * 해시맵 값의 크기
     */
    size_t of_size_value;

    /**
     * 키의 해싱에 사용되는 해시 함수
     */
    size_t (*hash)(const void* p);
    /**
     * 키의 비교에 사용되는 비교 함수
     */
    int (*comp)(const void* p, const void* q);
};

/**
 * *내부 함수
 *
 * @brief 해시맵의 크기를 두 배로 증가
 * @param ths 대상 해시맵 포인터
 */
void __hash_map_double(struct hash_map* ths);

/**
 * *내부 함수
 *
 * @brief 해시맵의 크기를 반으로 감소
 * @param ths 대상 해시맵 포인터
 */
void __hash_map_half(struct hash_map* ths);

/**
 * *참고: hash는 키의 해싱에 사용되는 함수로서, 해시맵의 크기에 적절한 충분히 큰 값을 반환해야 한다.
 *        comp는 qsort 등에서 사용되는 비교 함수와 마찬가지로 다음의 반환값을 가져야 한다.
 *        comp(p, q)에서:
 *            p >  q -> -1
 *            p == q ->  0
 *            p <  q ->  1
 *
 * @brief 새로운 해시맵 생성
 * @param of_size_key 키로 사용할 자료형의 크기
 * @param of_size_value 값으로 사용할 자료형의 크기
 * @param hash 키의 해싱에 사용되는 해시 함수
 * @param comp 키의 비교에 사용되는 해시 함수
 * @return
 */
struct hash_map* hash_map_create(size_t of_size_key, size_t of_size_value, size_t (*hash)(const void *p), int (*comp)(const void* p, const void* q));

/**
 * @brief 해시맵 삭제
 * @param ths 대상 해시맵 포인터
 */
void hash_map_delete(struct hash_map* ths);

/**
 * *참고: 로드 팩터는 해시맵의 한계 용량 대비 실제 요소의 대수의 비율이다.
 *        (K / N, K는 요소의 개수, N은 해시맵의 한계 용량)
 *
 *        본 해시맵은 로드 팩터가 0.7 초과이면 용량을 두 배로 늘린다.
 *        본 해시맵은 로드 팩터가 0.3 미만이면 용량을 반으로 줄인다.
 *        (단, 최소 용량은 21)
 *
 * @brief 해시맵의 로드 팩터 반환
 * @param ths 해시맵의 로드 팩터
 */
float hash_map_get_load_factor(struct hash_map* ths);

/**
 * 해시맵에 키에 대응하는 요소가 존재하지 않으면 새로 삽입한다.
 * 해시맵에 키에 대응하는 요소가 이미 존재하면 아무 것도 하지 않는다.
 *
 * @brief 해시맵에 새로운 요소 삽입
 * @param ths 대상 해시맵 포인터
 * @param key 키로 사용할 변수의 포인터
 * @param value 값으로 사용할 변수의 포인터
 */
void hash_map_push(struct hash_map* ths, void* key, void* value);

/**
 * @brief 해시맵에서 키에 대응하는 요소를 삭제
 * @param ths 대상 해시맵 포인터
 * @param key 키로 사용할 변수의 포인터
 */
void hash_map_pop(struct hash_map* ths, void* key);

/**
 * @brief 해시맵에서 키에 대응하는 값을 dest에 복사
 * @param ths 대상 해시맵 포인터
 * @param dest 값을 복사할 목적지
 * @param key 키로 사용할 변수의 포인터
 */
void hash_map_get(struct hash_map* ths, void* dest, void* key);

/**
 * @brief 해시맵에서 키에 대응하는 요소의 값을 value로 설정
 * @param ths 대상 해시맵 포인터
 * @param key 키로 사용할 변수의 포인터
 * @param value 요소의 값으로 사용할 변수의 포인터
 */
void hash_map_set(struct hash_map* ths, void* key, void* value);

/**
 * @brief 해시맵 초기화
 * @param ths 대상 해시맵 포인터
 */
void hash_map_clear(struct hash_map* ths);

void __hash_map_double(struct hash_map* ths)
{
    struct list** delete_arr = ths->arr;

    ths->capacity *= 2;
    ths->size = 0;

    ths->arr = (struct list**)malloc_s(ths->capacity * sizeof(struct list*));
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for hash map in __hash_map_double()\n");
        abort();
    }

    for (size_t i = 0; i < ths->capacity; i++)
        ths->arr[i] = list_create(sizeof(struct pair*));

    for (size_t i = 0; i < ths->capacity / 2; i++)
    {
        for (size_t j = 0; j < delete_arr[i]->size; j++)
        {
            struct pair* pair;
            list_get(delete_arr[i], &pair, j);
            hash_map_push(ths, pair->first, pair->second);
            pair_delete(pair);
        }
        list_delete(delete_arr[i]);
    }
    free_s(delete_arr);
}

void __hash_map_half(struct hash_map* ths)
{
    struct list** delete_arr = ths->arr;

    ths->capacity /= 2;
    ths->size = 0;

    ths->arr = (struct list**)malloc_s(ths->capacity * sizeof(struct list*));
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for hash map in __hash_map_half()\n");
        abort();
    }

    for (size_t i = 0; i < ths->capacity; i++)
        ths->arr[i] = list_create(sizeof(struct pair*));

    for (size_t i = 0; i < ths->capacity * 2; i++)
    {
        for (size_t j = 0; j < delete_arr[i]->size; j++)
        {
            struct pair* pair;
            list_get(delete_arr[i], &pair, j);
            hash_map_push(ths, pair->first, pair->second);
            pair_delete(pair);
        }
        list_delete(delete_arr[i]);
    }
    free_s(delete_arr);
}

struct hash_map* hash_map_create(size_t of_size_key, size_t of_size_value, size_t (*hash)(const void *p), int (*comp)(const void* p, const void* q))
{
    struct hash_map* ths = (struct hash_map*)malloc_s(sizeof(struct hash_map));

    ths->arr = (struct list**)malloc_s(21 * sizeof(struct list*));
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for hash map in hash_map_create()\n");
        abort();
    }

    for (int i = 0; i < 21; i++)
        ths->arr[i] = list_create(sizeof(struct pair*));

    ths->capacity = 21;
    ths->size = 0;
    ths->of_size_key = of_size_key;
    ths->of_size_value = of_size_value;

    ths->hash = hash;
    ths->comp = comp;

    return ths;
}

void hash_map_delete(struct hash_map* ths)
{
    for (size_t i = 0; i < ths->capacity; i++)
    {
        for (size_t j = 0; j < ths->arr[i]->size; j++)
            pair_delete(((struct pair**)ths->arr[i]->arr)[j]);
        list_delete(ths->arr[i]);
    }
    free_s(ths->arr);
    free_s(ths);
}

float hash_map_get_load_factor(struct hash_map* ths)
{
    return (float)ths->size / ths->capacity;
}

void hash_map_push(struct hash_map* ths, void* key, void* value)
{
    if (hash_map_get_load_factor(ths) > 0.7)
        __hash_map_double(ths);

    size_t index = ths->hash(key) % ths->capacity;

    for (size_t i = 0; i < ths->arr[index]->size; i++)
    {
        struct pair* pair = ((struct pair**)ths->arr[index]->arr)[i];
        if (ths->comp(key, pair->first) == 0)
        {
            return;
        }
    }

    struct pair* pair = pair_make(key, ths->of_size_key, value, ths->of_size_value);
    list_push(ths->arr[index], &pair);

    ths->size++;
}

void hash_map_pop(struct hash_map* ths, void* key)
{
    size_t index = ths->hash(key) % ths->capacity;

    for (size_t i = 0; i < ths->arr[index]->size; i++)
    {
        struct pair* pair = ((struct pair**)ths->arr[index]->arr)[i];
        if (ths->comp(key, pair->first) == 0)
        {
            pair_delete(pair);
            list_remove(ths->arr[index], i);
            ths->size--;

            if (ths->capacity > 21 && hash_map_get_load_factor(ths) < 0.3)
                __hash_map_half(ths);

            return;
        }
    }

    fprintf(stderr, "stderr: Failed to pop an element from hash map because key is invalid.\n");
    abort();
}

void hash_map_get(struct hash_map* ths, void* dest, void* key)
{
    size_t index = ths->hash(key) % ths->capacity;

    for (size_t i = 0; i < ths->arr[index]->size; i++)
    {
        struct pair* pair = ((struct pair**)ths->arr[index]->arr)[i];
        if (ths->comp(key, pair->first) == 0)
        {
            memcpy(dest, pair->second, ths->of_size_value);
            return;
        }
    }

    fprintf(stderr, "stderr: Failed to read an element from hash map because key is invalid.\n");
    abort();
}

void hash_map_set(struct hash_map* ths, void* key, void* value)
{
    size_t index = ths->hash(key) % ths->capacity;

    for (size_t i = 0; i < ths->arr[index]->size; i++)
    {
        struct pair* pair = ((struct pair**)ths->arr[index]->arr)[i];
        if (ths->comp(key, pair->first) == 0)
        {
            pair_set_second(pair, value);
            return;
        }
    }

    fprintf(stderr, "stderr: Failed to set an element in hash map because key is invalid.\n");
    abort();
}

void hash_map_clear(struct hash_map* ths)
{
    for (size_t i = 0; i < ths->capacity; i++)
    {
        size_t iter = ths->arr[i]->size;
        for (size_t j = 0; j < iter; j++)
        {
            struct pair* pair = ((struct pair**)ths->arr[i]->arr)[j];
            hash_map_pop(ths, pair->first);
        }
    }
}

#endif
