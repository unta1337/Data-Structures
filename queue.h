#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * 배열을 기반으로 구현된 가변 크기 큐
 */
struct queue
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

/**
 * *내부 함수
 * 
 * @brief 큐 크기 2배 증가
 * @param ths 대상 큐 포인터
 */
void __queue_double(struct queue* ths)
{
    ths->capacity *= 2;
    ths->arr = realloc(ths->arr, ths->capacity * ths->of_size);
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
void __queue_half(struct queue* ths)
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
    ths->arr = realloc(ths->arr, ths->capacity * ths->of_size);
    if (ths->arr == NULL)
    {
        fprintf(stderr, "stderr: Failed to reallocate memory for queue in __queue_half().\n");
        abort();
    }

    ths->head = 0;
    ths->tail = ths->capacity - 1;
}

/**
 * @brief 새로운 큐 생성
 * @param of_size 큐에 저장할 단일 요소의 크기
 * @return 동적으로 생성된 큐의 주소
 */
struct queue* queue_create(size_t of_size)
{
    struct queue* ths = (struct queue*)malloc(sizeof(struct queue));

    ths->arr = malloc(of_size);
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

/**
 * @brief 큐 삭제
 * @param ths 대상 큐 포인터
 */
void queue_delete(struct queue* ths)
{
    free(ths->arr);
    free(ths);
}

/**
 * @brief 큐이 비었는지 여부 반환
 * @param ths 대상 큐 포인터
 * @return 큐 빔 여부
 */
bool queue_empty(struct queue* ths)
{
    return ths->size == 0;
}

/**
 * @brief 큐에 새로운 요소 삽입
 * @param ths 대상 큐 포인터
 * @param value 삽입할 대상을 가리키는 포인터
 */
void queue_push(struct queue* ths, void* value)
{
    if (ths->size + 1 == ths->capacity)
        __queue_double(ths);

    memcpy((char*)ths->arr + ths->tail++ * ths->of_size, value, ths->of_size);
    ths->tail %= ths->capacity;

    ths->size++;
}

/**
 * @brief 큐의 첫 요소를 삭제
 * @param ths 대상 큐 포인터
 */
void queue_pop(struct queue* ths)
{
    if (queue_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to pop a element from queue because the queue is empty.\n");
        abort();
    }

    ths->head += ths->capacity + 1;
    ths->head %= ths->capacity;

    ths->size--;

    if (ths->size + 1 == ths->capacity / 2)
        __queue_half(ths);
}

/**
 * @brief 큐의 첫 요소를 dest에 복사
 * @param ths 대상 큐 포인터
 * @param dest 요소를 복사할 목적지
 */
void queue_front(struct queue* ths, void* dest)
{
    if (queue_empty(ths))
    {
        fprintf(stderr, "stderr: Failed to read from the top of the queue because the queue is empty.\n");
        abort();
    }

    memcpy(dest, (char*)ths->arr + ths->head * ths->of_size, ths->of_size);
}

/**
 * @brief 큐 초기화
 * @param ths 대상 큐 포인터
 */
void queue_clear(struct queue* ths)
{
    ths->arr = realloc(ths->arr, ths->of_size);
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

#endif