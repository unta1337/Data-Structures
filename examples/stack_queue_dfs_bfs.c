// 스택과 큐를 활용한 백준 DFS, BFS 문제 풀이.
// 1260 DFS와 BFS: https://www.acmicpc.net/problem/1260

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../stack.h"
#include "../queue.h"

struct stack* stk;
struct queue* q;

int n, m, v;
bool adjacent[1001][1001];

void dfs()
{
    bool visited[10001] = { false, };
    stack_push(stk, &v);

    while (!stack_empty(stk))
    {
        int vertex;
        stack_pop(stk, &vertex);

        if (visited[vertex])
            continue;
        visited[vertex] = true;

        printf("%d ", vertex);

        for (int i = n; i >= 1; i--)
            if (adjacent[vertex][i])
                stack_push(stk, &i);
    }

    printf("\n");
}

void bfs()
{
    bool visited[10001] = { false, };
    queue_push(q, &v);

    while (!queue_empty(q))
    {
        int vertex;
        queue_pop(q, &vertex);

        if (visited[vertex])
            continue;
        visited[vertex] = true;

        printf("%d ", vertex);

        for (int i = 1; i <= n; i++)
            if (adjacent[vertex][i])
                queue_push(q, &i);
    }

    printf("\n");
}

int main(void)
{
    scanf("%d %d %d", &n, &m, &v);

    stk = stack_create(m, sizeof(int));
    q = queue_create(m, sizeof(int));

    for (int i = 0; i < m; i++)
    {
        int v1, v2;
        scanf("%d %d", &v1, &v2);
        
        adjacent[v1][v2] = true;
        adjacent[v2][v1] = true;
    }

    dfs();
    bfs();

    stack_remove(stk);
    queue_remove(q);

    return 0;
}