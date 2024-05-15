// 저지 온라인: 백준
// 1927: 최소 힙
// URL: http://boj.kr/87c9b37610d441bc9ba67a1088759f49

#include <stdio.h>

#define UNDS_TRACK_MEM
#include "../unds_heap_queue.h"

int comp(const void* p, const void* q)
{
    int i = *(int*)p;
    int j = *(int*)q;

    return (i > j) - (i < j);
}

int N, input;
unds_heap_queue_t* hq;

int main(void)
{
    hq = unds_heap_queue_create(sizeof(int), comp);

    scanf("%d", &N);
    for (int i = 0; i < N; i++)
    {
        scanf("%d", &input);

        if (input)
            unds_heap_queue_push(hq, &input);
        else
        {
            if (unds_heap_queue_empty(hq))
            {
                printf("0\n");
                continue;
            }

            int temp;
            unds_heap_queue_front(hq, &temp);
            unds_heap_queue_pop(hq);
            printf("%d\n", temp);
        }
    }

    unds_heap_queue_delete(hq);

    printf("\nCurrent Memory Usage (should be 0): %zu.\n", unds_used_malloc);

    return 0;
}
