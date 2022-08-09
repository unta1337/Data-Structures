// 저지 온라인: 백준
// 1260: DFS와 BFS
// URL: http://boj.kr/98fa1b72871d4f379cbe35ae64760386

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// 참고, 아래의 include 경로는 본 리포지토리에 한하므로 실제 백준 제출 시에는 헤더 파일을 직접 삽입했다.
#include "../stack.h"
#include "../queue.h"

int main(void)
{
    int N, M, V;
    scanf("%d %d %d", &N, &M, &V);

    bool adjacent[1001][1001] = { false, };
    for (int i = 0; i < M; i++)
    {
        int x, y;
        scanf("%d %d", &x, &y);

        adjacent[x][y] = true;
        adjacent[y][x] = true;
    }

    // DFS
    struct stack* stk = stack_create(sizeof(int));
    stack_push(stk, &V);

    bool stack_visited[1001] = { false };

    while (!stack_empty(stk))
    {
        int vertex;
        stack_top(stk, &vertex);
        stack_pop(stk);

        if (stack_visited[vertex])
            continue;
        stack_visited[vertex] = true;

        printf("%d ", vertex);

        for (int i = N; i >= 1; i--)
            if (adjacent[vertex][i])
                stack_push(stk, &i);
    }
    printf("\n");

    stack_delete(stk);

    // BFS
    struct queue* q = queue_create(sizeof(int));
    queue_push(q, &V);

    bool queue_visited[1001] = { false };

    while (!queue_empty(q))
    {
        int vertex;
        queue_front(q, &vertex);
        queue_pop(q);

        if (queue_visited[vertex])
            continue;
        queue_visited[vertex] = true;

        printf("%d ", vertex);

        for (int i = 1; i <= N; i++)
            if (adjacent[vertex][i])
                queue_push(q, &i);
    }
    printf("\n");

    queue_delete(q);

    printf("\nCurrent Memory Usage (should be 0): %zu.\n", used_malloc);

    return 0;
}