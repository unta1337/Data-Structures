#ifndef __LIST_H
#define __LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct list
{
    void* arr;          // 실제 데이터를 저장할 공간에 대한 포인터.
    size_t capacity;    // 리스트의 한계 용량.
    size_t of_size;     // 리스트 단일 요소의 크기. (sizeof(arr[0]))
    size_t size;        // 리스트의 실제 크기. (저장된 요소의 개수)
};

/*
    새로운 리스트 생성

    @param of_size 리스트에 저장할 단일 요소의 크기
    @return 동적으로 생성된 리스트의 주소
 */
struct list* list_create(size_t of_size)
{
    struct list* ths = (struct list*)malloc(sizeof(struct list));

    ths->arr = malloc(of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to allocate memory for list in list_create().\n");
        abort();
        return NULL;
    }

    ths->capacity = 1;
    ths->of_size = of_size;
    ths->size = 0;

    return ths;
}

/*
    리스트 삭제

    @param ths 대상 리스트 포인터
 */
void list_delete(struct list* ths)
{
    free(ths->arr);
    free(ths);
}

/*
    *내부 함수
    리스트 크기 2배 증가

    @param ths 대상 리스트 포인터
 */
void __list_double(struct list* ths)
{
    ths->capacity *= 2;
    ths->arr = realloc(ths->arr, ths->capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for list in __list_double().\n");
        abort();
    }
}

/*
    *내부 함수
    리스트 크기 2배 감소

    @param ths 대상 리스트 포인터
 */
void __list_half(struct list* ths)
{
    ths->capacity /= 2;
    ths->arr = realloc(ths->arr, ths->capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for list in __list_half().\n");
        abort();
    }
}

/*
    리스트가 비었는지 여부 반환

    @param ths 대상 리스트 포인터
    @return 리스트 빔 여부
 */
bool list_empty(struct list* ths)
{
    return ths->size == 0;
}

/*
    리스트에 새로운 요소 삽입

    매개변수 value는 삽입할 대상을 가리키는 포인터 또는 주소여야 한다.
    int a를 삽입하고 싶다면 a에 대한 주소인 &a나 int* p = &a에서 p를 넘기면 된다.

    -> int a가 저장된 메모리상의 위치를 0x10라 하자.
       원하는 결과는 0x10에 저장된 값을 삽입하는 것이므로 이를 가리키는 포인터, 즉 a의 주솟값을 넘겨야 한다.

    int a를 가리키는 포인터 int* p를 삽입하고 싶다면 p의 주소나 int** q = &p에서 q를 넘기면 된다.

    -> int a가 저장된 메모리상의 위치를 0x10, int* p가 저장된 위치를 0x20이라 하자.
       원하는 결과는 a의 주소이므로 0x10을 삽입해야 한다.
       이를 저장하고 있는 주소는 p의 주소인 0x20이므로 &p 또는 int** q = &p의 q를 넘기면 된다.

    단, 배열의 포인터를 삽입할 때는 배열을 이름으로 참조한 주솟값과 & 연산자로 참조한 주솟값이 동일하다.
    따라서 배열을 가리키는 포인터의 주소를 넘겨야 한다.

    -> 배열을 복사하여 삽입하는 경우.
    -> int arr[3]가 저장된 메모리상의 위치를 0x10이라 하자.
       원하는 결과는 0x10로부터 3 * sizeof(int)만큼을 삽입하는 것이므로 배열의 이름을 그대로 넘기면 된다.

    -> 배열의 참조를 삽입하는 경우.
    -> int arr[3]가 저장된 메모리상의 위치를 0x10이라 하자.
       원하는 결과는 주솟값이 0x10을 삽입하는 것이다.
       그런데 배열은 arr과 &arr이 동일한 값을 가지므로 arr을 가리키는 별도의 포인터를 이용해야 한다.
       int* p = arr로 p가 arr의 주소를 가리키도록 하자.
       p는 배열이 아닌 포인터이므로 &p를 통해 0x10를 가리키는 주솟값을 얻을 수 있다.
       따라서 &p를 넘기면 된다.

    참고, int** 등의 이중 포인터 대신 void*를 매개변수로 넘겨도 된다.
    int** q = &p 대신 void* q = &p.

    @param ths 대상 리스트 포인터
    @param value 삽입할 대상을 가리키는 포인터
 */
void list_push(struct list* ths, void* value)
{
    if (ths->size == ths->capacity)
        __list_double(ths);

    memcpy((char*)ths->arr + ths->size++ * ths->of_size, value, ths->of_size);
}

/*
    리스트의 마지막 요소를 dest에 복사 후 삭제

    dest는 요소를 복사할 수 있을 만큼의 공간을 가리키는 포인터 또는 주소여야 한다.
    dest는 삽입한 요소를 담을 수 있는 적절한 포인터 자료형이거나 void*여야 한다.

    -> 단일 요소의 크기가 2 * sizeof(int), 즉 배열을 삽입한 경우 아래와 같이 이용할 수 있다.
       int temp[2];
       list_pop(lst, temp);

       또는,
       int* temp_v = malloc(2 * sizeof(int));
       list_pop(lst, temp_v);

    @param ths 대상 리스트 포인터
    @param dest 요소를 복사할 목적지
 */
void list_pop(struct list* ths, void* dest)
{
    if (list_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to pop a element from list because the list is empty.\n");
        abort();
    }

    if (ths->size == ths->capacity / 2)
        __list_half(ths);

    memcpy(dest, (char*)ths->arr + --ths->size * ths->of_size, ths->of_size);
}

/*
    리스트의 특정 요소를 dest에 복사

    @param ths 대상 리스트 포인터
    @param dest 요소를 복사할 목적지
    @param index 복사할 대상의 인덱스
    @return 복사 성공 여부
 */
void list_get(struct list* ths, void* dest, size_t index)
{
    if (index >= ths->size)
    {
        fprintf(stderr, "stderr: List index out of range. Expected less then %zu but found %zu.\n", ths->size, index);
        abort();
    }

    memcpy(dest, (char*)ths->arr + index * ths->of_size, ths->of_size);
}

/*
    리스트의 특정 요소 value로 변경

    @param ths 대상 리스트 포인터
    @param index 변경할 대상의 인덱스
    @param value 새로 삽입할 요소를 가리키는 포인터
 */
void list_set(struct list* ths, size_t index, void* value)
{
    if (index >= ths->size)
    {
        fprintf(stderr, "stderr: List index out of range. Expected less then %zu but found %zu.\n", ths->size, index);
        abort();
    }

    memcpy((char*)ths->arr + index * ths->of_size, value, ths->of_size);
}

/*
    리스트 초기화

    @param ths 대상 리스트 포인터
 */
void list_clear(struct list* ths)
{
    ths->arr = realloc(ths->arr, ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for list in list_clear().\n");
        abort();
    }

    ths->capacity = 1;
    ths->size = 0;
}

/*
    리스트 정렬

    @param ths 대상 리스트 포인터
    @param comp 정렬 기준을 정의하는 함수 (qsort의 comp와 동일)
 */
void list_sort(struct list* ths, int (*comp)(const void*, const void*))
{
    qsort(ths->arr, ths->size, ths->of_size, comp);
}

#endif