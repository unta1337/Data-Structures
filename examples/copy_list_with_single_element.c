#include <stdio.h>

#include "../list.h"

int main(void)
{
    // 대상의 복사본을 저장하는 리스트.
    struct list* lst = list_create(sizeof(int));

    int a = 10;
    int b = 20;
    int c = 30;

    list_push(lst, &a);
    list_push(lst, &b);
    list_push(lst, &c);

    printf("최초 리스트: \n");
    for (size_t i = 0; i < lst->size; i++)
    {
        // sizeof(int) 만큼의 단일 요소를 저장하므로 요소를 읽기 위해 sizeof(int) 만큼의 공간이 필요하다.
        // malloc 등을 이용해 공간을 할당하는 것도 가능하다.
        // e.g. int* temp = (int*)malloc(sizeof(int));
        int temp;
        list_get(lst, &temp, i);

        printf("%d ", temp);
    }
    printf("\n\n");

    // 복사본을 저장하므로 원본을 수정해도 리스트의 요소는 변하지 않음.
    // (참고, list_get 연산은 목적지 포인터에 값의 복사본을 전달하므로 리스트의 특정 요소를 수정할 수 없음)
    a = 100;

    printf("원본 수정 후 리스트: \n");
    for (size_t i = 0; i < lst->size; i++)
    {
        int temp;
        list_get(lst, &temp, i);

        printf("%d ", temp);
    }
    printf("\n");

    list_remove(lst);

    return 0;
}