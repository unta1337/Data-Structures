#include <stdio.h>
#include <limits.h>

#define UNDS_TRACK_MEM
#define UNDS_IMPLEMENTATION
#include "../unds.h"

#define INF INT_MAX / 4

#define NUM_V 7

int weights[NUM_V][NUM_V] =
{
    {   0,   7, INF, INF,   3,  10, INF },
    {   7,   0,   4,  10,   2,   6, INF },
    { INF,   4,   0,   2, INF, INF, INF },
    { INF,  10,   2,   0,  11,   9,   4 },
    {   3,   2, INF,  11,   0,  13,   5 },
    {  10,   6, INF,   9,  13,   0, INF },
    { INF, INF, INF,   4,   5, INF,   0 }
};

int comp(const void* p, const void* q)
{
    int i = ((int*)p)[1];
    int j = ((int*)q)[1];

    return (i > j) - (i < j);
}

int dijkstra(int start)
{
    int dist[NUM_V];
    for (int i = 0; i < NUM_V; i++) dist[i] = INF;
    dist[start] = 0;

    unds_heap_queue_t* hq = unds_heap_queue_create(2 * sizeof(int), comp);
    unds_heap_queue_push(hq, (int[]){ start, 0 });

    int path[NUM_V];
    for (int i = 0; i < NUM_V; i++) path[i] = start;

    while (!unds_heap_queue_empty(hq))
    {
        int temp[2];
        unds_heap_queue_front(hq, temp);
        unds_heap_queue_pop(hq);

        int vertex = temp[0];
        int cost = temp[1];

        if (cost > dist[vertex])
            continue;

        for (int i = 0; i < NUM_V; i++)
        {
            if (weights[vertex][i] == INF)
                continue;

            if (dist[vertex] + weights[vertex][i] < dist[i])
            {
                dist[i] = dist[vertex] + weights[vertex][i];
                path[i] = vertex;

                unds_heap_queue_push(hq, (int[]){ i, dist[i] });
            }
        }
    }

    printf("Path from %d to every vertex (reverse order):\n", start);
    for (int i = 0; i < NUM_V; i++)
    {
        if (i == start)
            continue;

        printf("    Dist: %2d | ", dist[i]);

        int index = i;
        while (index != start)
        {
            printf("%2d ", index);
            index = path[index];
        }
        printf("%2d \n", start);
    }
    printf("\n");

    unds_heap_queue_delete(hq);

    return 0;
}

int main(void)
{
    for (int i = 0; i < NUM_V; i++)
        dijkstra(i);

    printf("\nCurrent Memory Usage (should be 0): %zu.\n", unds_used_malloc);

    return 0;
}
