#ifndef __UNDS_HASH_MAP_H
#define __UNDS_HASH_MAP_H

#include "unds_list.h"
#include "unds_pair.h"

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
 * 리스트와 페어를 기반으로 구현된 가변 크기 해시맵
 */
struct unds_hash_map_t
{
    /**
     * 실제 데이터를 저장하기 위한 리스트
     */
    unds_list_t** arr;
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

typedef struct unds_hash_map_t unds_hash_map_t;

/**
 * *내부 함수
 *
 * @brief 해시맵의 크기를 두 배로 증가
 * @param ths 대상 해시맵 포인터
 */
void __unds_hash_map_double(unds_hash_map_t* ths);

/**
 * *내부 함수
 *
 * @brief 해시맵의 크기를 반으로 감소
 * @param ths 대상 해시맵 포인터
 */
void __unds_hash_map_half(unds_hash_map_t* ths);

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
 * @return 동적으로 생성된 해시맵 포인터
 */
unds_hash_map_t* unds_hash_map_create(size_t of_size_key, size_t of_size_value, size_t (*hash)(const void *p), int (*comp)(const void* p, const void* q));

/**
 * @brief 해시맵 삭제
 * @param ths 대상 해시맵 포인터
 */
void unds_hash_map_delete(unds_hash_map_t* ths);

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
float unds_hash_map_get_load_factor(unds_hash_map_t* ths);

/**
 * 해시맵에 키에 대응하는 요소가 존재하지 않으면 새로 삽입한다.
 * 해시맵에 키에 대응하는 요소가 이미 존재하면 아무 것도 하지 않는다.
 *
 * @brief 해시맵에 새로운 요소 삽입
 * @param ths 대상 해시맵 포인터
 * @param key 키로 사용할 변수의 포인터
 * @param value 값으로 사용할 변수의 포인터
 */
void unds_hash_map_push(unds_hash_map_t* ths, void* key, void* value);

/**
 * @brief 해시맵에서 키에 대응하는 요소를 삭제
 * @param ths 대상 해시맵 포인터
 * @param key 키로 사용할 변수의 포인터
 */
void unds_hash_map_pop(unds_hash_map_t* ths, void* key);

/**
 * @brief 해시맵에서 키에 대응하는 값을 dest에 복사
 * @param ths 대상 해시맵 포인터
 * @param dest 값을 복사할 목적지
 * @param key 키로 사용할 변수의 포인터
 */
void unds_hash_map_get(unds_hash_map_t* ths, void* dest, void* key);

/**
 * @brief 해시맵에서 키에 대응하는 요소의 값을 value로 설정
 * @param ths 대상 해시맵 포인터
 * @param key 키로 사용할 변수의 포인터
 * @param value 요소의 값으로 사용할 변수의 포인터
 */
void unds_hash_map_set(unds_hash_map_t* ths, void* key, void* value);

/**
 * @brief 해시맵에 키에 대응하는 요소가 있는지 검사
 * @param ths 대상 해시맵 포인터
 * @param 키로 사용할 변수의 포인터
 * @return 키에 대응하는 요소의 존재 유무
 */
bool unds_hash_map_has(unds_hash_map_t* ths, void* key);

/**
 * @brief 해시맵 초기화
 * @param ths 대상 해시맵 포인터
 */
void unds_hash_map_clear(unds_hash_map_t* ths);

void __unds_hash_map_double(unds_hash_map_t* ths)
{
    unds_list_t** delete_arr = ths->arr;

    ths->capacity *= 2;
    ths->size = 0;

    ths->arr = (unds_list_t**)unds_malloc(ths->capacity * sizeof(unds_list_t*));
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for hash map in __hash_map_double()\n");
        abort();
    }

    for (size_t i = 0; i < ths->capacity; i++)
        ths->arr[i] = unds_list_create(sizeof(unds_pair_t*));

    for (size_t i = 0; i < ths->capacity / 2; i++)
    {
        for (size_t j = 0; j < delete_arr[i]->size; j++)
        {
            unds_pair_t* pair;
            unds_list_get(delete_arr[i], &pair, j);
            unds_hash_map_push(ths, pair->first, pair->second);
            unds_pair_delete(pair);
        }
        unds_list_delete(delete_arr[i]);
    }
    unds_free(delete_arr);
}

void __unds_hash_map_half(unds_hash_map_t* ths)
{
    unds_list_t** delete_arr = ths->arr;

    ths->capacity /= 2;
    ths->size = 0;

    ths->arr = (unds_list_t**)unds_malloc(ths->capacity * sizeof(unds_list_t*));
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for hash map in __hash_map_half()\n");
        abort();
    }

    for (size_t i = 0; i < ths->capacity; i++)
        ths->arr[i] = unds_list_create(sizeof(unds_pair_t*));

    for (size_t i = 0; i < ths->capacity * 2; i++)
    {
        for (size_t j = 0; j < delete_arr[i]->size; j++)
        {
            unds_pair_t* pair;
            unds_list_get(delete_arr[i], &pair, j);
            unds_hash_map_push(ths, pair->first, pair->second);
            unds_pair_delete(pair);
        }
        unds_list_delete(delete_arr[i]);
    }
    unds_free(delete_arr);
}

unds_hash_map_t* unds_hash_map_create(size_t of_size_key, size_t of_size_value, size_t (*hash)(const void *p), int (*comp)(const void* p, const void* q))
{
    unds_hash_map_t* ths = (unds_hash_map_t*)unds_malloc(sizeof(unds_hash_map_t));

    ths->arr = (unds_list_t**)unds_malloc(21 * sizeof(unds_list_t*));
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for hash map in hash_map_create()\n");
        abort();
    }

    for (int i = 0; i < 21; i++)
        ths->arr[i] = unds_list_create(sizeof(unds_pair_t*));

    ths->capacity = 21;
    ths->size = 0;
    ths->of_size_key = of_size_key;
    ths->of_size_value = of_size_value;

    ths->hash = hash;
    ths->comp = comp;

    return ths;
}

void unds_hash_map_delete(unds_hash_map_t* ths)
{
    for (size_t i = 0; i < ths->capacity; i++)
    {
        for (size_t j = 0; j < ths->arr[i]->size; j++)
            unds_pair_delete(((unds_pair_t**)ths->arr[i]->arr)[j]);
        unds_list_delete(ths->arr[i]);
    }
    unds_free(ths->arr);
    unds_free(ths);
}

float unds_hash_map_get_load_factor(unds_hash_map_t* ths)
{
    return (float)ths->size / ths->capacity;
}

void unds_hash_map_push(unds_hash_map_t* ths, void* key, void* value)
{
    if (unds_hash_map_get_load_factor(ths) > 0.7)
        __unds_hash_map_double(ths);

    size_t index = ths->hash(key) % ths->capacity;

    for (size_t i = 0; i < ths->arr[index]->size; i++)
    {
        unds_pair_t* pair = ((unds_pair_t**)ths->arr[index]->arr)[i];
        if (ths->comp(key, pair->first) == 0)
        {
            return;
        }
    }

    unds_pair_t* pair = unds_pair_make(key, ths->of_size_key, value, ths->of_size_value);
    unds_list_push(ths->arr[index], &pair);

    ths->size++;
}

void unds_hash_map_pop(unds_hash_map_t* ths, void* key)
{
    size_t index = ths->hash(key) % ths->capacity;

    for (size_t i = 0; i < ths->arr[index]->size; i++)
    {
        unds_pair_t* pair = ((unds_pair_t**)ths->arr[index]->arr)[i];
        if (ths->comp(key, pair->first) == 0)
        {
            unds_pair_delete(pair);
            unds_list_remove(ths->arr[index], i);
            ths->size--;

            if (ths->capacity > 21 && unds_hash_map_get_load_factor(ths) < 0.3)
                __unds_hash_map_half(ths);

            return;
        }
    }

    fprintf(stderr, "stderr: Failed to pop an element from hash map because key is invalid.\n");
    abort();
}

void unds_hash_map_get(unds_hash_map_t* ths, void* dest, void* key)
{
    size_t index = ths->hash(key) % ths->capacity;

    for (size_t i = 0; i < ths->arr[index]->size; i++)
    {
        unds_pair_t* pair = ((unds_pair_t**)ths->arr[index]->arr)[i];
        if (ths->comp(key, pair->first) == 0)
        {
            memcpy(dest, pair->second, ths->of_size_value);
            return;
        }
    }

    fprintf(stderr, "stderr: Failed to read an element from hash map because key is invalid.\n");
    abort();
}

void unds_hash_map_set(unds_hash_map_t* ths, void* key, void* value)
{
    size_t index = ths->hash(key) % ths->capacity;

    for (size_t i = 0; i < ths->arr[index]->size; i++)
    {
        unds_pair_t* pair = ((unds_pair_t**)ths->arr[index]->arr)[i];
        if (ths->comp(key, pair->first) == 0)
        {
            unds_pair_set_second(pair, value);
            return;
        }
    }

    fprintf(stderr, "stderr: Failed to set an element in hash map because key is invalid.\n");
    abort();
}

bool unds_hash_map_has(unds_hash_map_t* ths, void* key)
{
    size_t index = ths->hash(key) % ths->capacity;

    for (size_t i = 0; i < ths->arr[index]->size; i++)
    {
        unds_pair_t* pair = ((unds_pair_t**)ths->arr[index]->arr)[i];
        if (ths->comp(key, pair->first) == 0)
            return true;
    }

    return false;
}

void unds_hash_map_clear(unds_hash_map_t* ths)
{
    for (size_t i = 0; i < ths->capacity; i++)
    {
        size_t iter = ths->arr[i]->size;
        for (size_t j = 0; j < iter; j++)
        {
            unds_pair_t* pair = ((unds_pair_t**)ths->arr[i]->arr)[j];
            unds_hash_map_pop(ths, pair->first);
        }
    }
}

#endif
