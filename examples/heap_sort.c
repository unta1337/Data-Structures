#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../heap_queue.h"

#define NUM_ARR (1 << 8)

int comp(const void* p, const void* q)
{
    int i = *(int*)p;
    int j = *(int*)q;

    return (i > j) - (i < j);
}

int is_sorted(int* arr, size_t size)
{
    for (size_t i = 0; i < size - 1; i++)
        if (comp(&arr[i], &arr[i + 1]) == 1)
            return 0;
    return 1;
}

int main(void)
{
    srand((unsigned int)time(NULL));

    int arr[NUM_ARR];
    for (int i = 0; i < NUM_ARR; i++)
        arr[i] = rand() % NUM_ARR - NUM_ARR / 2;

    printf("Initial Array:\n");
    for (int i = 0; i < NUM_ARR; i++)
        printf("%d ", arr[i]);
    printf("\n");

    struct heap_queue* hq = heap_queue_create_from_array(arr, NUM_ARR, sizeof(int), comp);

    int index = 0;
    while (hq->size > 0)
    {
        heap_queue_front(hq, &arr[index++]);
        heap_queue_pop(hq);
    }

    printf("Sorted Array:\n");
    for (int i = 0; i < NUM_ARR; i++)
        printf("%d ", arr[i]);
    printf("\n");

    printf("%s.\n", is_sorted(arr, NUM_ARR) ? "Sorted" : "Unsorted");

    heap_queue_delete(hq);

    return 0;
}