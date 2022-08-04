// 저지 온라인: 백준
// 11286: 절댓값 힙
// URL: http://boj.kr/1a74cc38613b450db2d07de06c1f5d7c

#include <stdio.h>

#include "../heap_queue.h"

int comp(const void* p, const void* q)
{
    int i = *(int*)p;
    int j = *(int*)q;

    if (abs(i) == abs(j))
        return (i > j) - (i < j);
    else
    {
        i = abs(i);
        j = abs(j);

        return (i > j) - (i < j);
    }
}

int N, input;
struct heap_queue* hq;

int main(void)
{
    hq = heap_queue_create(sizeof(int), comp);

    scanf("%d", &N);
    for (int i = 0; i < N; i++)
    {
        scanf("%d", &input);

        if (input)
            heap_queue_push(hq, &input);
        else
        {
            if (heap_queue_empty(hq))
            {
                printf("0\n");
                continue;
            }

            int temp;
            heap_queue_front(hq, &temp);
            heap_queue_pop(hq);
            printf("%d\n", temp);
        }
    }

    return 0;
}