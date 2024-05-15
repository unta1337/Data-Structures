#ifndef __UNDS_LIST_H
#define __UNDS_LIST_H

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

/**
 * @brief 새로운 리스트 생성
 * @param of_size 리스트에 저장할 단일 요소의 크기
 * @return 동적으로 생성된 리스트의 주소
 */
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

/**
 * @brief 배열로부터 새로운 리스트 생성
 * @param arr 리스트로 생성할 배열의 포인터
 * @param size 리스트로 생성할 배열의 길이
 * @param of_size 리스트로 생성할 배열의 단일 요소의 크기
 * @return 동적으로 생성된 리스트의 주소
 */
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

/**
 * @brief 기본값을 설정하여 새로운 리스트 생성
 * @param value 리스트의 기본값으로 설정할 값의 포인터
 * @param size 생성할 리스트의 길이
 * @param of_size 리스트의 기본값으로 설정할 값의 크기
 * @return 동적으로 생성된 리스트의 주소
 */
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

/**
 * @brief 리스트 삭제
 * @param ths 대상 리스트 포인터
 */
void unds_list_delete(unds_list_t* ths)
{
    unds_free(ths->arr);
    unds_free(ths);
}

/**
 * @brief 리스트가 비었는지 여부 반환
 * @param ths 대상 리스트 포인터
 * @return 리스트 빔 여부
 */
bool unds_list_empty(unds_list_t* ths)
{
    return ths->size == 0;
}

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
void unds_list_push(unds_list_t* ths, void* value)
{
    if (ths->size == ths->capacity)
        __unds_list_double(ths);

    memcpy((char*)ths->arr + ths->size++ * ths->of_size, value, ths->of_size);
}

/**
 * @brief 리스트의 마지막 요소를 삭제
 * @param ths 대상 리스트 포인터
 */
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
void unds_list_get(unds_list_t* ths, void* dest, size_t index)
{
    if (index >= ths->size)
    {
        fprintf(stderr, "stderr: List index out of range. Expected less then %zu but found %zu.\n", ths->size, index);
        abort();
    }

    memcpy(dest, (char*)ths->arr + index * ths->of_size, ths->of_size);
}

/**
 * @brief 리스트의 특정 요소 value로 변경
 * @param ths 대상 리스트 포인터
 * @param index 변경할 대상의 인덱스
 * @param value 새로 삽입할 요소를 가리키는 포인터
 */
void unds_list_set(unds_list_t* ths, size_t index, void* value)
{
    if (index >= ths->size)
    {
        fprintf(stderr, "stderr: List index out of range. Expected less then %zu but found %zu.\n", ths->size, index);
        abort();
    }

    memcpy((char*)ths->arr + index * ths->of_size, value, ths->of_size);
}

/**
 * @brief 리스트의 특정 위치에 요소 추가
 * @param ths 대상 리스트 포인터
 * @param index 새로운 요소를 추가할 위치
 * @param value 새로 삽입할 요소를 가리키는 포인터
 */
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

/**
 * @brief 리스트의 특정 위치의 요소 삭제
 * @param ths 대상 리스트 포인터
 * @param index 삭제할 요소의 인덱스
 */
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

/**
 * @brief 리스트 초기화
 * @param ths 대상 리스트 포인터
 */
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

/**
 * @brief 리스트 정렬
 * @param ths 대상 리스트 포인터
 * @param comp 정렬 기준을 정의하는 함수 (qsort의 comp와 동일)
 */
void unds_list_sort(unds_list_t* ths, int (*comp)(const void*, const void*))
{
    qsort(ths->arr, ths->size, ths->of_size, comp);
}

#endif
