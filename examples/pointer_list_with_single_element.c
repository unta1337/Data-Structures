#include <stdio.h>

#include "../list.h"

int main(void)
{
    // 대상의 포인터를 저장하는 리스트.
    struct list* lst = list_create(sizeof(int*));

    int a = 10;
    int b = 20;
    int c = 30;

    // 리스트에 삽입할 단일 요소의 크기는 리스트를 생성할 때 결정되므로 포인터의 자료형을 중요하지 않다.
    // int*처럼 자료형을 명시해도 되고 아래와 같이 void*를 이용해도 좋다.
    void* p = &a;
    void* q = &b;
    void* r = &c;

    list_push(lst, &p);
    list_push(lst, &q);
    list_push(lst, &r);

    printf("최초 리스트: \n");
    for (size_t i = 0; i < lst->size; i++)
    {
        // 참고, 포인터 변수는 선언 시 포인터가 가리키는 공간의 주소를 저장할 공간만을 마련한다.
        //       따라서 실제 값에 대한 공간을 따로 할당해야 한다.
        // 이 역시 삽입 시와 마찬가지로 int**와 같이 자료형을 명시해도 되고 void*를 이용해도 좋다.
        // 참고, int**가 되는 이유는 리스트가 포인터를 저장하므로 리스트에 저장된 포인터가 가리키는 변수를 읽으려면 역참조를 두 번 해야 한다.
        //       즉, 이중 포인터가 필요하다.
        void* temp = malloc(sizeof(int*));

        list_get(lst, temp, i);

        int* temp_int = *(void**)temp;
        printf("%d ", *temp_int);

        free(temp);
    }
    printf("\n\n");

    // 포인터를 저장하므로 원본을 수정하면 리스트의 요소가 변함.
    // (참고, list_get 연산은 목적지 포인터에 값의 복사본을 전달하므로 리스트의 특정 요소를 수정할 수 없음)
    // (본 예제는 포인터를 전달하므로 포인터가 가리키는 '변수'의 값을 변경하는 것은 가능하나 포인터 그 자체를 변경하여 다른 변수를 가리키도록 할 수는 없음)
    a = 100;

    printf("원본 수정 후 리스트: \n");
    for (size_t i = 0; i < lst->size; i++)
    {
        void* temp = malloc(sizeof(int*));

        list_get(lst, temp, i);

        int* temp_int = *(void**)temp;
        printf("%d ", *temp_int);

        free(temp);
    }
    printf("\n");

    list_remove(lst);

    return 0;
}