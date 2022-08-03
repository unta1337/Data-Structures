// 저지 온라인: 백준
// 10845: 큐
// URL: http://boj.kr/602bdce2418d44f1a13da6bbd94c6593

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 참고, 아래의 include 경로는 본 리포지토리에 한하므로 실제 백준 제출 시에는 헤더 파일을 직접 삽입했다.
#include "../queue.h"

int N;
char input[20];
struct queue* q;

int main(void)
{
    q = queue_create(sizeof(int));

    scanf("%d\n", &N);
    for (int i = 0; i < N; i++)
    {
        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = '\0';

        char* cmd = strtok(input, " ");
        int temp;

        if (!strcmp(cmd, "push"))
        {
            char* operand = strtok(NULL, " ");
            temp = strtol(operand, NULL, 10);
            queue_push(q, &temp);
        }
        else if (!strcmp(cmd, "pop"))
        {
            if (queue_empty(q))
            {
                printf("-1\n");
                continue;
            }

            queue_front(q, &temp);
            queue_pop(q);
            printf("%d\n", temp);
        }
        else if (!strcmp(cmd, "size"))
        {
            printf("%zu\n", q->size);
        }
        else if (!strcmp(cmd, "empty"))
        {
            printf("%d\n", queue_empty(q));
        }
        else if (!strcmp(cmd, "front"))
        {
            if (queue_empty(q))
            {
                printf("-1\n");
                continue;
            }

            queue_front(q, &temp);
            printf("%d\n", temp);
        }
        else if (!strcmp(cmd, "back"))
        {
            if (queue_empty(q))
            {
                printf("-1\n");
                continue;
            }

            queue_back(q, &temp);
            printf("%d\n", temp);
        }
    }

    return 0;
}