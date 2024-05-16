#ifndef __UNDS_H
#define __UNDS_H

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
unds_pair_t* unds_pair_create(size_t of_size_first, size_t of_size_second);

/**
 * @brief 초기값을 설정하여 페어 생성
 * @param first 첫 번재 요소
 * @param of_size_first 첫 번째 요소의 크기
 * @param second 두 번재 요소
 * @param of_size_first 두 번째 요소의 크기
 * @return 동적할당된 페어의 포인터
 */
unds_pair_t* unds_pair_make(void* first, size_t of_size_first, void* second, size_t of_size_second);

/**
 * @brief 페어 삭제
 * @param 삭제할 페어의 포인터
 */
void unds_pair_delete(unds_pair_t* ths);

/**
 * @brief 페어의 첫 번재 요소를 dest에 복사
 * @param ths 대상 페어 포인터
 * @param dest 복사할 목적지
 */
void unds_pair_get_first(unds_pair_t* ths, void* dest);

/**
 * @brief 페어의 두 번재 요소를 dest에 복사
 * @param ths 대상 페어 포인터
 * @param dest 복사할 목적지
 */
void unds_pair_get_second(unds_pair_t* ths, void* dest);

/**
 * @brief 페어의 첫 번재 요소를 value로 설정
 * @param ths 대상 페어 포인터
 * @param value 설정할 값
 */
void unds_pair_set_first(unds_pair_t* ths, void* value);

/**
 * @brief 페어의 두 번재 요소를 value로 설정
 * @param ths 대상 페어 포인터
 * @param value 설정할 값
 */
void unds_pair_set_second(unds_pair_t* ths, void* value);

/**
 * 배열을 기반으로 구현된 가변 길이 리스트
 */
struct unds_list_t
{
    /**
     * 실제 데이터를 저장할 공간에 대한 포인터
     */
    void* arr;
    /**
     * 리스트의 한계 용량
     */
    size_t capacity;
    /**
     * 리스트의 길이
     */
    size_t size;
    /**
     * 리스트 단일 요소의 크기
     */
    size_t of_size;
};

typedef struct unds_list_t unds_list_t;

/**
 * @brief 새로운 리스트 생성
 * @param of_size 리스트에 저장할 단일 요소의 크기
 * @return 동적으로 생성된 리스트의 주소
 */
unds_list_t* unds_list_create(size_t of_size);

/**
 * @brief 배열로부터 새로운 리스트 생성
 * @param arr 리스트로 생성할 배열의 포인터
 * @param size 리스트로 생성할 배열의 길이
 * @param of_size 리스트로 생성할 배열의 단일 요소의 크기
 * @return 동적으로 생성된 리스트의 주소
 */
unds_list_t* unds_list_create_from_array(void* arr, size_t size, size_t of_size);

/**
 * @brief 기본값을 설정하여 새로운 리스트 생성
 * @param value 리스트의 기본값으로 설정할 값의 포인터
 * @param size 생성할 리스트의 길이
 * @param of_size 리스트의 기본값으로 설정할 값의 크기
 * @return 동적으로 생성된 리스트의 주소
 */
unds_list_t* unds_list_create_from_value(void* value, size_t size, size_t of_size);

/**
 * @brief 리스트 삭제
 * @param ths 대상 리스트 포인터
 */
void unds_list_delete(unds_list_t* ths);

/**
 * @brief 리스트가 비었는지 여부 반환
 * @param ths 대상 리스트 포인터
 * @return 리스트 빔 여부
 */
bool unds_list_empty(unds_list_t* ths);

/**
 * 매개변수 value는 삽입할 대상을 가리키는 포인터 또는 주소여야 한다.
 * int a를 삽입하고 싶다면 a에 대한 주소인 &a나 int* p = &a에서 p를 넘기면 된다.
 *
 * -> int a가 저장된 메모리상의 위치를 0x10라 하자.
 *    원하는 결과는 0x10에 저장된 값을 삽입하는 것이므로 이를 가리키는 포인터, 즉 a의 주솟값을 넘겨야 한다.
 *
 * int a를 가리키는 포인터 int* p를 삽입하고 싶다면 p의 주소나 int** q = &p에서 q를 넘기면 된다.
 *
 * -> int a가 저장된 메모리상의 위치를 0x10, int* p가 저장된 위치를 0x20이라 하자.
 *    원하는 결과는 a의 주소이므로 0x10을 삽입해야 한다.
 *    이를 저장하고 있는 주소는 p의 주소인 0x20이므로 &p 또는 int** q = &p의 q를 넘기면 된다.
 *
 * 단, 배열의 포인터를 삽입할 때는 배열을 이름으로 참조한 주솟값과 & 연산자로 참조한 주솟값이 동일하다.
 * 따라서 배열을 가리키는 포인터의 주소를 넘겨야 한다.
 *
 * -> 배열을 복사하여 삽입하는 경우.
 * -> int arr[3]가 저장된 메모리상의 위치를 0x10이라 하자.
 *    원하는 결과는 0x10로부터 3 * sizeof(int)만큼을 삽입하는 것이므로 배열의 이름을 그대로 넘기면 된다.
 *
 * -> 배열의 참조를 삽입하는 경우.
 * -> int arr[3]가 저장된 메모리상의 위치를 0x10이라 하자.
 *    원하는 결과는 주솟값이 0x10을 삽입하는 것이다.
 *    그런데 배열은 arr과 &arr이 동일한 값을 가지므로 arr을 가리키는 별도의 포인터를 이용해야 한다.
 *    int* p = arr로 p가 arr의 주소를 가리키도록 하자.
 *    p는 배열이 아닌 포인터이므로 &p를 통해 0x10를 가리키는 주솟값을 얻을 수 있다.
 *    따라서 &p를 넘기면 된다.
 *
 * 참고, int** 등의 이중 포인터 대신 void*를 매개변수로 넘겨도 된다.
 * int** q = &p 대신 void* q = &p.
 *
 * @brief 리스트에 새로운 요소 삽입
 * @param ths 대상 리스트 포인터
 * @param value 삽입할 대상을 가리키는 포인터
 */
void unds_list_push(unds_list_t* ths, void* value);

/**
 * @brief 리스트의 마지막 요소를 삭제
 * @param ths 대상 리스트 포인터
 */
void unds_list_pop(unds_list_t* ths);

/**
 * dest는 요소를 복사할 수 있을 만큼의 공간을 가리키는 포인터 또는 주소여야 한다.
 * dest는 삽입한 요소를 담을 수 있는 적절한 포인터 자료형이거나 void*여야 한다.
 *
 * -> 단일 요소의 크기가 2 * sizeof(int), 즉 배열을 삽입한 경우 아래와 같이 이용할 수 있다.
 *    int temp[2];
 *    list_pop(lst, temp);
 *
 *    또는,
 *    int* temp_v = unds_malloc(2 * sizeof(int));
 *    list_pop(lst, temp_v);
 *
 * @brief 리스트의 특정 요소를 dest에 복사
 * @param ths 대상 리스트 포인터
 * @param dest 요소를 복사할 목적지
 * @param index 복사할 대상의 인덱스
 * @return 복사 성공 여부
 */
void unds_list_get(unds_list_t* ths, void* dest, size_t index);

/**
 * @brief 리스트의 특정 요소 value로 변경
 * @param ths 대상 리스트 포인터
 * @param index 변경할 대상의 인덱스
 * @param value 새로 삽입할 요소를 가리키는 포인터
 */
void unds_list_set(unds_list_t* ths, size_t index, void* value);

/**
 * @brief 리스트의 특정 위치에 요소 추가
 * @param ths 대상 리스트 포인터
 * @param index 새로운 요소를 추가할 위치
 * @param value 새로 삽입할 요소를 가리키는 포인터
 */
void unds_list_insert(unds_list_t* ths, size_t index, void* value);

/**
 * @brief 리스트의 특정 위치의 요소 삭제
 * @param ths 대상 리스트 포인터
 * @param index 삭제할 요소의 인덱스
 */
void unds_list_remove(unds_list_t* ths, size_t index);

/**
 * @brief 리스트 초기화
 * @param ths 대상 리스트 포인터
 */
void unds_list_clear(unds_list_t* ths);

/**
 * @brief 리스트 정렬
 * @param ths 대상 리스트 포인터
 * @param comp 정렬 기준을 정의하는 함수 (qsort의 comp와 동일)
 */
void unds_list_sort(unds_list_t* ths, int (*comp)(const void*, const void*));

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
 * @brief 새로운 덱 생성
 * @param of_size 덱에 저장할 단일 요소의 크기
 * @return 동적으로 생성된 덱의 주소
 */
unds_deque_t* unds_deque_create(size_t of_size);

/**
 * @brief 배열로부터 새로운 덱 생성
 * @param arr 덱으로 생성할 배열의 포인터
 * @param size 덱으로 생성할 배열의 길이
 * @param of_size 덱으로 생성할 배열의 단일 요소의 크기
 * @return 동적으로 생성된 덱의 주소
 */
unds_deque_t* unds_deque_create_from_array(void* arr, size_t size, size_t of_size);

/**
 * @brief 기본값을 설정하여 새로운 덱 생성
 * @param value 덱의 기본값으로 설정할 값의 포인터
 * @param size 생성할 덱의 길이
 * @param of_size 덱의 기본값으로 설정할 값의 크기
 * @return 동적으로 생성된 덱의 주소
 */
unds_deque_t* unds_deque_create_from_value(void* value, size_t size, size_t of_size);

/**
 * @brief 덱 삭제
 * @param ths 대상 덱 포인터
 */
void unds_deque_delete(unds_deque_t* ths);

/**
 * @brief 덱이 비었는지 여부 반환
 * @param ths 대상 덱 포인터
 * @return 덱 빔 여부
 */
bool unds_deque_empty(unds_deque_t* ths);

/**
 * @brief 덱에 앞부분에 새로운 요소 삽입
 * @param ths 대상 덱 포인터
 * @param value 삽입할 대상을 가리키는 포인터
 */
void unds_deque_push_front(unds_deque_t* ths, void* value);

/**
 * @brief 덱에 뒷부분에 새로운 요소 삽입
 * @param ths 대상 덱 포인터
 * @param value 삽입할 대상을 가리키는 포인터
 */
void unds_deque_push_back(unds_deque_t* ths, void* value);

/**
 * @brief 덱의 마지막 요소를 삭제
 * @param ths 대상 덱 포인터
 */
void unds_deque_pop_front(unds_deque_t* ths);

/**
 * @brief 덱의 마지막 요소를 삭제
 * @param ths 대상 덱 포인터
 */
void unds_deque_pop_back(unds_deque_t* ths);

/**
 * @brief 덱의 첫 요소를 dest에 복사
 * @param ths 대상 덱 포인터
 * @param dest 요소를 복사할 목적지
 */
void unds_deque_front(unds_deque_t* ths, void* dest);

/**
 * @brief 덱의 마지막 요소를 dest에 복사
 * @param ths 대상 덱 포인터
 * @param dest 요소를 복사할 목적지
 */
void unds_deque_back(unds_deque_t* ths, void* dest);

/**
 * @brief 덱 초기화
 * @param ths 대상 덱 포인터
 */
void unds_deque_clear(unds_deque_t* ths);

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

/**
 * 배열을 기반으로 구현된 가변 크기 힙큐
 */
struct unds_heap_queue_t
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

typedef struct unds_heap_queue_t unds_heap_queue_t;

/**
 * @brief 새로운 힙큐 생성
 * @param of_size 힙큐에 저장할 단일 요소의 크기
 * @param comp 힙큐 연산 시에 사용되는 요소 비교 함수
 * @return 동적으로 생성된 힙큐의 주소
 */
unds_heap_queue_t* unds_heap_queue_create(size_t of_size, int (*comp)(const void* p, const void* q));

/**
 * @brief 배열로부터 새로운 힙큐 생성
 * @param arr 힙큐로 생성할 배열의 포인터
 * @param size 힙큐로 생성할 배열의 길이
 * @param of_size 힙큐로 생성할 배열의 단일 요소의 크기
 * @return 동적으로 생성된 힙큐의 주소
 */
unds_heap_queue_t* unds_heap_queue_create_from_array(void* arr, size_t size, size_t of_size, int (*comp)(const void* p, const void* q));

/**
 * @brief 기본값을 설정하여 새로운 힙큐 생성
 * @param value 힙큐의 기본값으로 설정할 값의 포인터
 * @param size 생성할 힙큐의 길이
 * @param of_size 힙큐의 기본값으로 설정할 값의 크기
 * @return 동적으로 생성된 힙큐의 주소
 */
unds_heap_queue_t* unds_heap_queue_create_from_value(void* value, size_t size, size_t of_size, int (*comp)(const void* p, const void* q));

/**
 * @brief 힙큐 삭제
 * @param ths 대상 힙큐 포인터
 */
void unds_heap_queue_delete(unds_heap_queue_t* ths);

/**
 * @brief 힙큐가 비었는지 여부 반환
 * @param ths 대상 힙큐 포인터
 * @return 힙큐 빔 여부
 */
bool unds_heap_queue_empty(unds_heap_queue_t* ths);

/**
 * @brief 힙큐에 새로운 요소 삽입
 * @param ths 대상 덱 포인터
 * @param value 삽입할 대상을 가리키는 포인터
 */
void unds_heap_queue_push(unds_heap_queue_t* ths, void* value);

/**
 * @brief 힙큐의 첫 요소를 삭제
 * @param ths 대상 힙큐 포인터
 */
void unds_heap_queue_pop(unds_heap_queue_t* ths);

/**
 * @brief 힙큐의 첫 요소를 dest에 복사
 * @param ths 대상 힙큐 포인터
 * @param dest 요소를 복사할 목적지
 */
void unds_heap_queue_front(unds_heap_queue_t* ths, void* dest);

/**
 * @brief 힙큐 초기화
 * @param ths 대상 힙큐 포인터
 */
void unds_heap_queue_clear(unds_heap_queue_t* ths);

/**
 * 배열을 기반으로 구현된 가변 크기 큐
 */
struct unds_queue_t
{
    /**
     * 실제 데이터를 저장할 공간에 대한 포인터
     */
    void* arr;
    /**
     * 큐의 한계 용량
     */
    size_t capacity;
    /**
     * 큐의 크기
     */
    size_t size;
    /**
     * 큐 단일 요소의 크기
     */
    size_t of_size;
    /**
     * 큐의 앞부분을 나타내는 인덱스
     */
    size_t head;
    /**
     * 큐의 뒷부분을 나타내는 인덱스
     */
    size_t tail;
};

typedef struct unds_queue_t unds_queue_t;

/**
 * @brief 새로운 큐 생성
 * @param of_size 큐에 저장할 단일 요소의 크기
 * @return 동적으로 생성된 큐의 주소
 */
unds_queue_t* unds_queue_create(size_t of_size);

/**
 * @brief 배열로부터 새로운 큐 생성
 * @param arr 큐로 생성할 배열의 포인터
 * @param size 큐로 생성할 배열의 길이
 * @param of_size 큐로 생성할 배열의 단일 요소의 크기
 * @return 동적으로 생성된 큐의 주소
 */
unds_queue_t* unds_queue_create_from_array(void* arr, size_t size, size_t of_size);

/**
 * @brief 기본값을 설정하여 새로운 큐 생성
 * @param value 큐의 기본값으로 설정할 값의 포인터
 * @param size 생성할 큐의 길이
 * @param of_size 큐의 기본값으로 설정할 값의 크기
 * @return 동적으로 생성된 큐의 주소
 */
unds_queue_t* unds_queue_create_from_value(void* value, size_t size, size_t of_size);

/**
 * @brief 큐 삭제
 * @param ths 대상 큐 포인터
 */
void unds_queue_delete(unds_queue_t* ths);

/**
 * @brief 큐이 비었는지 여부 반환
 * @param ths 대상 큐 포인터
 * @return 큐 빔 여부
 */
bool unds_queue_empty(unds_queue_t* ths);

/**
 * @brief 큐에 새로운 요소 삽입
 * @param ths 대상 큐 포인터
 * @param value 삽입할 대상을 가리키는 포인터
 */
void unds_queue_push(unds_queue_t* ths, void* value);

/**
 * @brief 큐의 첫 요소를 삭제
 * @param ths 대상 큐 포인터
 */
void unds_queue_pop(unds_queue_t* ths);

/**
 * @brief 큐의 첫 요소를 dest에 복사
 * @param ths 대상 큐 포인터
 * @param dest 요소를 복사할 목적지
 */
void unds_queue_front(unds_queue_t* ths, void* dest);

/**
 * @brief 큐의 마지막 요소를 dest에 복사
 * @param ths 대상 큐 포인터
 * @param dest 요소를 복사할 목적지
 */
void unds_queue_back(unds_queue_t* ths, void* dest);

/**
 * @brief 큐 초기화
 * @param ths 대상 큐 포인터
 */
void unds_queue_clear(unds_queue_t* ths);

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
 * @brief 새로운 스택 생성
 * @param of_size 스택에 저장할 단일 요소의 크기
 * @return 동적으로 생성된 스택의 주소
 */
unds_stack_t* unds_stack_create(size_t of_size);

/**
 * @brief 배열로부터 새로운 스택 생성
 * @param arr 스택으로 생성할 배열의 포인터
 * @param size 스택으로 생성할 배열의 길이
 * @param of_size 스택으로 생성할 배열의 단일 요소의 크기
 * @return 동적으로 생성된 스택의 주소
 */
unds_stack_t* unds_stack_create_from_array(void* arr, size_t size, size_t of_size);

/**
 * @brief 기본값을 설정하여 새로운 스택 생성
 * @param value 스택의 기본값으로 설정할 값의 포인터
 * @param size 생성할 스택의 길이
 * @param of_size 스택의 기본값으로 설정할 값의 크기
 * @return 동적으로 생성된 스택의 주소
 */
unds_stack_t* unds_stack_create_from_value(void* value, size_t size, size_t of_size);

/**
 * @brief 스택 삭제
 * @param ths 대상 스택 포인터
 */
void unds_stack_delete(unds_stack_t* ths);

/**
 * @brief 스택이 비었는지 여부 반환
 * @param ths 대상 스택 포인터
 * @return 스택 빔 여부
 */
bool unds_stack_empty(unds_stack_t* ths);

/**
 * @brief 스택에 새로운 요소 삽입
 * @param ths 대상 스택 포인터
 * @param value 삽입할 대상을 가리키는 포인터
 */
void unds_stack_push(unds_stack_t* ths, void* value);

/**
 * @brief 스택의 마지막 요소를 삭제
 * @param ths 대상 스택 포인터
 */
void unds_stack_pop(unds_stack_t* ths);

/**
 * @brief 스택의 마지막 요소를 dest에 복사
 * @param ths 대상 스택 포인터
 * @param dest 요소를 복사할 목적지
 */
void unds_stack_top(unds_stack_t* ths, void* dest);

/**
 * @brief 스택 초기화
 * @param ths 대상 스택 포인터
 */
void unds_stack_clear(unds_stack_t* ths);

#ifdef UNDS_IMPLEMENTATION

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

void unds_pair_delete(unds_pair_t* ths)
{
    unds_free(ths->first);
    unds_free(ths->second);
    unds_free(ths);
}

void unds_pair_get_first(unds_pair_t* ths, void* dest)
{
    memcpy(dest, ths->first, ths->of_size_first);
}

void unds_pair_get_second(unds_pair_t* ths, void* dest)
{
    memcpy(dest, ths->second, ths->of_size_second);
}

void unds_pair_set_first(unds_pair_t* ths, void* value)
{
    memcpy(ths->first, value, ths->of_size_first);
}

void unds_pair_set_second(unds_pair_t* ths, void* value)
{
    memcpy(ths->second, value, ths->of_size_second);
}

/**
 * *내부 함수
 *
 * @brief 리스트 크기 2배 증가
 * @param ths 대상 리스트 포인터
 */
void __unds_list_double(unds_list_t* ths)
{
    ths->capacity *= 2;
    ths->arr = unds_realloc(ths->arr, ths->capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for list in __list_double().\n");
        abort();
    }
}

/**
 * *내부 함수
 *
 * @brief 리스트 크기 2배 감소
 * @param ths 대상 리스트 포인터
 */
void __unds_list_half(unds_list_t* ths)
{
    if (ths->size == 0)
        return;

    ths->capacity /= 2;
    ths->arr = unds_realloc(ths->arr, ths->capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for list in __list_half().\n");
        abort();
    }
}

/**
 * *내부 함수
 *
 * @brief 리스트의 크기를 2의 제곱수로 교정
 * @param ths 대상 리스트 포인터
 */
void __unds_list_capacity_correction(unds_list_t* ths)
{
    size_t correct_capacity = 1;

    while (correct_capacity <= ths->size)
        correct_capacity *= 2;

    ths->arr = unds_realloc(ths->arr, correct_capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for stack in __list_capacity_correction().\n");
        abort();
    }

    ths->capacity = correct_capacity;
}

unds_list_t* unds_list_create(size_t of_size)
{
    unds_list_t* ths = (unds_list_t*)unds_malloc(sizeof(unds_list_t));

    ths->arr = unds_malloc(of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for list in list_create().\n");
        abort();
    }

    ths->capacity = 1;
    ths->size = 0;
    ths->of_size = of_size;

    return ths;
}

unds_list_t* unds_list_create_from_array(void* arr, size_t size, size_t of_size)
{
    if (arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to initialize list since the original array is NULL.\n");
        abort();
    }
    else if (of_size == 0)
    {
        fprintf(stderr, "stderr: Size of a single element of list cannot be zero.");
        abort();
    }

    unds_list_t* ths = (unds_list_t*)unds_malloc(sizeof(unds_list_t));

    ths->arr = unds_malloc(size * of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for list in list_create_from_array().\n");
        abort();
    }

    ths->capacity = size;
    ths->size = size;
    ths->of_size = of_size;

    memcpy(ths->arr, arr, size * of_size);
    __unds_list_capacity_correction(ths);

    return ths;
}

unds_list_t* unds_list_create_from_value(void* value, size_t size, size_t of_size)
{
    if (value == NULL)
    {
        fprintf(stderr, "stderr: Failed to initialize list since the original array is NULL.\n");
        abort();
    }
    else if (of_size == 0)
    {
        fprintf(stderr, "stderr: Size of a single element of list cannot be zero.");
        abort();
    }

    unds_list_t* ths = (unds_list_t*)unds_malloc(sizeof(unds_list_t));
    ths->arr = unds_malloc(size * of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for list in list_create_from_array().\n");
        abort();
    }

    ths->capacity = size;
    ths->size = size;
    ths->of_size = of_size;

    for (size_t i = 0; i < size; i++)
        memcpy((char*)ths->arr + i * of_size, value, of_size);
    __unds_list_capacity_correction(ths);
    
    return ths;
}

void unds_list_delete(unds_list_t* ths)
{
    unds_free(ths->arr);
    unds_free(ths);
}

bool unds_list_empty(unds_list_t* ths)
{
    return ths->size == 0;
}

void unds_list_push(unds_list_t* ths, void* value)
{
    if (ths->size == ths->capacity)
        __unds_list_double(ths);

    memcpy((char*)ths->arr + ths->size++ * ths->of_size, value, ths->of_size);
}

void unds_list_pop(unds_list_t* ths)
{
    if (unds_list_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to pop an element from list because the list is empty.\n");
        abort();
    }

    ths->size--;

    if (ths->size == ths->capacity / 2)
        __unds_list_half(ths);
}

void unds_list_get(unds_list_t* ths, void* dest, size_t index)
{
    if (index >= ths->size)
    {
        fprintf(stderr, "stderr: List index out of range. Expected less then %zu but found %zu.\n", ths->size, index);
        abort();
    }

    memcpy(dest, (char*)ths->arr + index * ths->of_size, ths->of_size);
}

void unds_list_set(unds_list_t* ths, size_t index, void* value)
{
    if (index >= ths->size)
    {
        fprintf(stderr, "stderr: List index out of range. Expected less then %zu but found %zu.\n", ths->size, index);
        abort();
    }

    memcpy((char*)ths->arr + index * ths->of_size, value, ths->of_size);
}

void unds_list_insert(unds_list_t* ths, size_t index, void* value)
{
    if (index >= ths->size + 1)
    {
        fprintf(stderr, "stderr: List index out of range. Expected less then %zu but found %zu.\n", ths->size, index);
        abort();
    }

    void* temp = unds_malloc(ths->of_size);
    unds_list_push(ths, temp);
    unds_free(temp);

    memmove((char*)ths->arr + (index + 1) * ths->of_size, (char*)ths->arr + index * ths->of_size, (ths->size - index - 1) * ths->of_size);
    memcpy((char*)ths->arr + index * ths->of_size, value, ths->of_size);
}

void unds_list_remove(unds_list_t* ths, size_t index)
{
    if (index >= ths->size)
    {
        fprintf(stderr, "stderr: List index out of range. Expected less then %zu but found %zu.\n", ths->size, index);
        abort();
    }

    memmove((char*)ths->arr + index * ths->of_size, (char*)ths->arr + (index + 1) * ths->of_size, (ths->size - index) * ths->of_size);
    unds_list_pop(ths);
}

void unds_list_clear(unds_list_t* ths)
{
    ths->arr = unds_realloc(ths->arr, ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for list in list_clear().\n");
        abort();
    }

    ths->capacity = 1;
    ths->size = 0;
}

void unds_list_sort(unds_list_t* ths, int (*comp)(const void*, const void*))
{
    qsort(ths->arr, ths->size, ths->of_size, comp);
}

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

unds_deque_t* unds_deque_create_from_value(void* value, size_t size, size_t of_size)
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

void unds_deque_delete(unds_deque_t* ths)
{
    unds_free(ths->arr);
    unds_free(ths);
}

bool unds_deque_empty(unds_deque_t* ths)
{
    return ths->size == 0;
}

void unds_deque_push_front(unds_deque_t* ths, void* value)
{
    if (ths->size + 1 == ths->capacity)
        __unds_deque_double(ths);

    ths->head += ths->capacity - 1;
    ths->head %= ths->capacity;

    memcpy((char*)ths->arr + ths->head * ths->of_size, value, ths->of_size);

    ths->size++;
}

void unds_deque_push_back(unds_deque_t* ths, void* value)
{
    if (ths->size + 1 == ths->capacity)
        __unds_deque_double(ths);

    memcpy((char*)ths->arr + ths->tail * ths->of_size, value, ths->of_size);

    ths->tail++;
    ths->tail %= ths->capacity;

    ths->size++;
}

void unds_deque_pop_front(unds_deque_t* ths)
{
    if (unds_deque_empty(ths))
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

void unds_deque_pop_back(unds_deque_t* ths)
{
    if (unds_deque_empty(ths))
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

void unds_deque_front(unds_deque_t* ths, void* dest)
{
    if (unds_deque_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to read from the front of the deque because the deque is empty.\n");
        abort();
    }

    memcpy(dest, (char*)ths->arr + ths->head * ths->of_size, ths->of_size);
}

void unds_deque_back(unds_deque_t* ths, void* dest)
{
    if (unds_deque_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to read from the back of the deque because the deque is empty.\n");
        abort();
    }

    memcpy(dest, (char*)ths->arr + (ths->tail + ths->capacity - 1) % ths->capacity * ths->of_size, ths->of_size);
}

void unds_deque_clear(unds_deque_t* ths)
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

unds_hash_map_t* unds_hash_map_create(size_t of_size_key, size_t of_size_value, size_t (*hash)(const void *p), int (*comp)(const void* p, const void* q));

/**
 * *내부 함수
 *
 * @brief 해시맵의 크기를 두 배로 증가
 * @param ths 대상 해시맵 포인터
 */
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

/**
 * *내부 함수
 *
 * @brief 해시맵의 크기를 반으로 감소
 * @param ths 대상 해시맵 포인터
 */
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

/**
 * *내부 변수.
 *
 * 내부 true
 */
bool __unds_true = true;
/**
 * *내부 변수.
 *
 * 내부 false
 */
bool __unds_false = false;

/**
 * *내부 함수.
 * 
 * @brief 해시셋의 속성을 해시맵과 동기화
 * @param ths 대상 해시셋 포인터
 */
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

/**
 * *내부 함수
 *
 * @brief 힙큐 크기 2배 증가
 * @param ths 대상 힙큐 포인터
 */
void __unds_heap_queue_double(unds_heap_queue_t* ths)
{
    ths->capacity *= 2;
    ths->arr = unds_realloc(ths->arr, ths->capacity * ths->of_size);
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
void __unds_heap_queue_half(unds_heap_queue_t* ths)
{
    if (ths->size == 0)
        return;

    ths->capacity /= 2;
    ths->arr = unds_realloc(ths->arr, ths->capacity * ths->of_size);
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
void __unds_heap_queue_capacity_correction(unds_heap_queue_t* ths)
{
    size_t correct_capacity = 1;

    while (correct_capacity <= ths->size)
        correct_capacity *= 2;

    ths->arr = unds_realloc(ths->arr, correct_capacity * ths->of_size);
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
size_t __unds_heap_queue_get_left_child(size_t index)
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
size_t __unds_heap_queue_get_parent(size_t index)
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
void __unds_heap_queue_reheap_up(unds_heap_queue_t* ths, size_t index)
{
    size_t node = index;
    size_t parent = __unds_heap_queue_get_parent(node);

    while (index > 0)
    {
        if (ths->comp((char*)ths->arr + node * ths->of_size, (char*)ths->arr + parent * ths->of_size) != -1)
            break;

        void* temp = unds_malloc(ths->of_size);
        memcpy(temp, (char*)ths->arr + node * ths->of_size, ths->of_size);
        memcpy((char*)ths->arr + node * ths->of_size, (char*)ths->arr + parent * ths->of_size, ths->of_size);
        memcpy((char*)ths->arr + parent * ths->of_size, temp, ths->of_size);
        unds_free(temp);

        node = parent;
        parent = __unds_heap_queue_get_parent(node);
    }
}

/**
 * *내부 함수
 * 
 * @brief 특정 요소에 대해서 최하위 노드 방향으로 힙큐의 배열을 힙으로 변환
 * @param ths 대상 힙큐 포인터
 * @param index 힙으로 변환할 기준이 되는 요소의 인덱스
 */
void __unds_heap_queue_reheap_down(unds_heap_queue_t* ths, size_t index)
{
    size_t node = index;
    size_t child = __unds_heap_queue_get_left_child(index);

    while (child < ths->size)
    {
        int comp_result = -1;
        if (child + 1 < ths->size)
            comp_result = ths->comp((char*)ths->arr + child * ths->of_size, (char*)ths->arr + (child + 1) * ths->of_size);

        int higher_priority = comp_result == -1 ? child : child + 1;

        if (ths->comp((char*)ths->arr + higher_priority * ths->of_size, (char*)ths->arr + node * ths->of_size) != -1)
            break;

        void* temp = unds_malloc(ths->of_size);
        memcpy(temp, (char*)ths->arr + higher_priority * ths->of_size, ths->of_size);
        memcpy((char*)ths->arr + higher_priority * ths->of_size, (char*)ths->arr + node * ths->of_size, ths->of_size);
        memcpy((char*)ths->arr + node * ths->of_size, temp, ths->of_size);
        unds_free(temp);

        node = higher_priority;
        child = __unds_heap_queue_get_left_child(node);
    }
}

/**
 * *내부 함수
 * 
 * @brief 힙큐의 배열을 힙으로 변환
 * @param ths 대상 힙큐 포인터
 */
void __unds_heap_queue_heapify(unds_heap_queue_t* ths)
{
    if (ths->size == 0)
        return;

    size_t index = ths->size - 1;
    if (index % 2 == 1)
        index++;

    while (index > 0)
    {
        size_t parent = __unds_heap_queue_get_parent(index);
        __unds_heap_queue_reheap_down(ths, parent);
        index -= 2;
    }
}

unds_heap_queue_t* unds_heap_queue_create(size_t of_size, int (*comp)(const void* p, const void* q))
{
    unds_heap_queue_t* ths = (unds_heap_queue_t*)unds_malloc(sizeof(unds_heap_queue_t));

    ths->arr = unds_malloc(of_size);
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

unds_heap_queue_t* unds_heap_queue_create_from_array(void* arr, size_t size, size_t of_size, int (*comp)(const void* p, const void* q))
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

    unds_heap_queue_t* ths = (unds_heap_queue_t*)unds_malloc(sizeof(unds_heap_queue_t));

    ths->arr = unds_malloc(size * of_size);
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
    __unds_heap_queue_capacity_correction(ths);
    __unds_heap_queue_heapify(ths);

    return ths;
}

unds_heap_queue_t* unds_heap_queue_create_from_value(void* value, size_t size, size_t of_size, int (*comp)(const void* p, const void* q))
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

    unds_heap_queue_t* ths = (unds_heap_queue_t*)unds_malloc(sizeof(unds_heap_queue_t));
    ths->arr = unds_malloc(size * of_size);
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
    __unds_heap_queue_capacity_correction(ths);
    
    return ths;
}

void unds_heap_queue_delete(unds_heap_queue_t* ths)
{
    unds_free(ths->arr);
    unds_free(ths);
}

bool unds_heap_queue_empty(unds_heap_queue_t* ths)
{
    return ths->size == 0;
}

void unds_heap_queue_push(unds_heap_queue_t* ths, void* value)
{
    if (ths->size == ths->capacity)
        __unds_heap_queue_double(ths);

    memcpy((char*)ths->arr + ths->size * ths->of_size, value, ths->of_size);
    __unds_heap_queue_reheap_up(ths, ths->size);

    ths->size++;
}

void unds_heap_queue_pop(unds_heap_queue_t* ths)
{
    if (unds_heap_queue_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to pop an element from heap queue because the heap queue is empty.\n");
        abort();
    }

    ths->size--;

    memcpy(ths->arr, (char*)ths->arr + ths->size * ths->of_size, ths->of_size);
    __unds_heap_queue_reheap_down(ths, 0);

    if (ths->size == ths->capacity / 2)
        __unds_heap_queue_half(ths);
}

void unds_heap_queue_front(unds_heap_queue_t* ths, void* dest)
{
    if (unds_heap_queue_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to read from the front of the heap queue because the heap queue is empty.\n");
        abort();
    }

    memcpy(dest, ths->arr, ths->of_size);
}

void unds_heap_queue_clear(unds_heap_queue_t* ths)
{
    ths->arr = unds_realloc(ths->arr, ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for stack in heap_queue_clear().\n");
        abort();
    }

    ths->capacity = 1;
    ths->size = 0;
}

/**
 * *내부 함수
 * 
 * @brief 큐 크기 2배 증가
 * @param ths 대상 큐 포인터
 */
void __unds_queue_double(unds_queue_t* ths)
{
    ths->capacity *= 2;
    ths->arr = unds_realloc(ths->arr, ths->capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for queue in __queue_double().\n");
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
 * @brief 큐 크기 2배 감소
 * @param ths 대상 큐 포인터
 */
void __unds_queue_half(unds_queue_t* ths)
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
        fprintf(stderr, "stderr: Failed to reallocate memory for queue in __queue_half().\n");
        abort();
    }

    ths->head = 0;
    ths->tail = ths->capacity - 1;
}

/**
 * *내부 함수
 *
 * @brief 큐의 크기를 2의 제곱수로 교정
 * @param ths 대상 큐 포인터
 */
void __unds_queue_capacity_correction(unds_queue_t* ths)
{
    size_t correct_capacity = 1;

    while (correct_capacity <= ths->size + 1)
        correct_capacity *= 2;

    ths->arr = unds_realloc(ths->arr, correct_capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for queue in __queue_capacity_correction().\n");
        abort();
    }

    ths->capacity = correct_capacity;
}

unds_queue_t* unds_queue_create(size_t of_size)
{
    unds_queue_t* ths = (unds_queue_t*)unds_malloc(sizeof(unds_queue_t));

    ths->arr = unds_malloc(of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for queue in queue_create().\n");
        abort();
    }

    ths->capacity = 1;
    ths->size = 0;
    ths->of_size = of_size;
    ths->head = 0;
    ths->tail = 0;

    return ths;
}

unds_queue_t* unds_queue_create_from_array(void* arr, size_t size, size_t of_size)
{
    if (arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to initialize queue since the original array is NULL.\n");
        abort();
    }
    else if (of_size == 0)
    {
        fprintf(stderr, "stderr: Size of a single element of queue cannot be zero.");
        abort();
    }

    unds_queue_t* ths = (unds_queue_t*)unds_malloc(sizeof(unds_queue_t));

    ths->arr = unds_malloc(size * of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for queue in queue_create_from_array().\n");
        abort();
    }

    ths->capacity = size;
    ths->size = size;
    ths->of_size = of_size;
    ths->head = 0;
    ths->tail = size;

    memcpy(ths->arr, arr, size * of_size);
    __unds_queue_capacity_correction(ths);

    return ths;
}

unds_queue_t* unds_queue_create_from_value(void* value, size_t size, size_t of_size)
{
    if (value == NULL)
    {
        fprintf(stderr, "stderr: Failed to initialize queue since the original array is NULL.\n");
        abort();
    }
    else if (of_size == 0)
    {
        fprintf(stderr, "stderr: Size of a single element of queue cannot be zero.");
        abort();
    }

    unds_queue_t* ths = (unds_queue_t*)unds_malloc(sizeof(unds_queue_t));
    ths->arr = unds_malloc(size * of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for queue in queue_create_from_array().\n");
        abort();
    }

    ths->capacity = size;
    ths->size = size;
    ths->of_size = of_size;
    ths->head = 0;
    ths->tail = size;

    for (size_t i = 0; i < size; i++)
        memcpy((char*)ths->arr + i * of_size, value, of_size);
    __unds_queue_capacity_correction(ths);
    
    return ths;
}

void unds_queue_delete(unds_queue_t* ths)
{
    unds_free(ths->arr);
    unds_free(ths);
}

bool unds_queue_empty(unds_queue_t* ths)
{
    return ths->size == 0;
}

void unds_queue_push(unds_queue_t* ths, void* value)
{
    if (ths->size + 1 == ths->capacity)
        __unds_queue_double(ths);

    memcpy((char*)ths->arr + ths->tail * ths->of_size, value, ths->of_size);

    ths->tail++;
    ths->tail %= ths->capacity;

    ths->size++;
}

void unds_queue_pop(unds_queue_t* ths)
{
    if (unds_queue_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to pop an element from queue because the queue is empty.\n");
        abort();
    }

    ths->head++;
    ths->head %= ths->capacity;

    ths->size--;

    if (ths->size + 1 == ths->capacity / 2)
        __unds_queue_half(ths);
}

void unds_queue_front(unds_queue_t* ths, void* dest)
{
    if (unds_queue_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to read from the front of the queue because the queue is empty.\n");
        abort();
    }

    memcpy(dest, (char*)ths->arr + ths->head * ths->of_size, ths->of_size);
}

void unds_queue_back(unds_queue_t* ths, void* dest)
{
    if (unds_queue_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to read from the back of the queue because the queue is empty.\n");
        abort();
    }

    memcpy(dest, (char*)ths->arr + (ths->tail + ths->capacity - 1) % ths->capacity * ths->of_size, ths->of_size);
}

void unds_queue_clear(unds_queue_t* ths)
{
    ths->arr = unds_realloc(ths->arr, ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for stack in queue_clear().\n");
        abort();
    }

    ths->capacity = 1;
    ths->size = 0;
    ths->head = 0;
    ths->tail = 0;
}

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

void unds_stack_delete(unds_stack_t* ths)
{
    unds_free(ths->arr);
    unds_free(ths);
}

bool unds_stack_empty(unds_stack_t* ths)
{
    return ths->size == 0;
}

void unds_stack_push(unds_stack_t* ths, void* value)
{
    if (ths->size == ths->capacity)
        __unds_stack_double(ths);

    memcpy((char*)ths->arr + ths->size++ * ths->of_size, value, ths->of_size);
}

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

void unds_stack_top(unds_stack_t* ths, void* dest)
{
    if (unds_stack_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to read from the top of the stack because the stack is empty.\n");
        abort();
    }

    memcpy(dest, (char*)ths->arr + (ths->size - 1) * ths->of_size, ths->of_size);
}

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

#endif
