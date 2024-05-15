// 저지 온라인: 백준
// 10866: 덱
// URL: http://boj.kr/4964c89175904f44a7ec0bc9ffeea275

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 참고, 아래의 include 경로는 본 리포지토리에 한하므로 실제 백준 제출 시에는 헤더 파일을 직접 삽입했다.
#define UNDS_TRACK_MEM
#define UNDS_IMPLEMENTATION
#include "../unds.h"

int N;
char input[20];
unds_deque_t* deq;

int main(void)
{
    deq = unds_deque_create(sizeof(int));

    scanf("%d\n", &N);
    for (int i = 0; i < N; i++)
    {
        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = '\0';

        char* cmd = strtok(input, " ");
        int temp;

        if (!strcmp(cmd, "push_front"))
        {
            char* operand = strtok(NULL, " ");
            temp = strtol(operand, NULL, 10);
            unds_deque_push_front(deq, &temp);
        }
        else if (!strcmp(cmd, "push_back"))
        {
            char* operand = strtok(NULL, " ");
            temp = strtol(operand, NULL, 10);
            unds_deque_push_back(deq, &temp);
        }
        else if (!strcmp(cmd, "pop_front"))
        {
            if (unds_deque_empty(deq))
            {
                printf("-1\n");
                continue;
            }

            unds_deque_front(deq, &temp);
            unds_deque_pop_front(deq);
            printf("%d\n", temp);
        }
        else if (!strcmp(cmd, "pop_back"))
        {
            if (unds_deque_empty(deq))
            {
                printf("-1\n");
                continue;
            }

            unds_deque_back(deq, &temp);
            unds_deque_pop_back(deq);
            printf("%d\n", temp);
        }
        else if (!strcmp(cmd, "size"))
        {
            printf("%zu\n", deq->size);
        }
        else if (!strcmp(cmd, "empty"))
        {
            printf("%d\n", unds_deque_empty(deq));
        }
        else if (!strcmp(cmd, "front"))
        {
            if (unds_deque_empty(deq))
            {
                printf("-1\n");
                continue;
            }

            unds_deque_front(deq, &temp);
            printf("%d\n", temp);
        }
        else if (!strcmp(cmd, "back"))
        {
            if (unds_deque_empty(deq))
            {
                printf("-1\n");
                continue;
            }

            unds_deque_back(deq, &temp);
            printf("%d\n", temp);
        }
    }

    unds_deque_delete(deq);

    printf("\nCurrent Memory Usage (should be 0): %zu.\n", unds_used_malloc);

    return 0;
}
