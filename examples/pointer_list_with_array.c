#include <stdio.h>

#include "../list.h"

int main(void)
{
    // 배열의 복사본을 저장하는 리스트. (원소의 크기가 2 * sizeof(int)인 것에 주의)
    struct list* lst = list_create(2 * sizeof(int));

    // 참고, 배열은 그 자체로 포인터이므로 아래와 같이 배열 리터럴의 형태로 전달할 수 있다.
    list_push(lst, (int[]){ 1, 2 });
    list_push(lst, (int[]){ 3, 4 });

    // 변수를 이용한 삽입.
    int elem3[2] = { 5, 6 };
    list_push(lst, elem3);

    printf("최초 리스트: \n");
    for (size_t i = 0; i < lst->size; i++)
    {
        // 2 * sizeof(int) 만큼의 단일 요소를 저장하므로 요소를 읽기 위해 2 * sizeof(int) 만큼의 공간이 필요하다.
        // malloc 등을 이용해 공간을 할당하는 것도 가능하다.
        // e.g. int* temp = (int*)malloc(2 * sizeof(int));
        int temp[2];
        list_get(lst, temp, i);

        printf("%d %d\n", temp[0], temp[1]);
    }
    printf("\n");

    // 복사본을 저장하므로 원본을 수정해도 리스트의 요소는 변하지 않음.
    // (참고, list_get 연산은 목적지 포인터에 값의 복사본을 전달하므로 리스트의 특정 요소를 수정할 수 없음)
    elem3[0] = 50;

    printf("원본 수정 후 리스트: \n");
    for (size_t i = 0; i < lst->size; i++)
    {
        int temp[2];
        list_get(lst, temp, i);

        printf("%d %d\n", temp[0], temp[1]);
    }
    printf("\n");

    list_remove(lst);

    return 0;
}