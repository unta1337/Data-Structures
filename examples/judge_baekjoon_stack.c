#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../stack.h"

int N;
char input[20];
struct stack* stk;

int main(void)
{
    stk = stack_create(sizeof(int));

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
            stack_push(stk, &temp);
        }
        else if (!strcmp(cmd, "pop"))
        {
            if (stack_empty(stk))
            {
                printf("-1\n");
                continue;
            }

            stack_top(stk, &temp);
            stack_pop(stk);
            printf("%d\n", temp);
        }
        else if (!strcmp(cmd, "size"))
        {
            printf("%zu\n", stk->size);
        }
        else if (!strcmp(cmd, "empty"))
        {
            printf("%d\n", stack_empty(stk));
        }
        else if (!strcmp(cmd, "top"))
        {
            if (stack_empty(stk))
            {
                printf("-1\n");
                continue;
            }

            stack_top(stk, &temp);
            printf("%d\n", temp);
        }
    }

    stack_delete(stk);

    printf("\nCurrent Memory Usage (should be 0): %zu.\n", used_malloc);

    return 0;
}