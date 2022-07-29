#include <stdio.h>

#include "../list.h"

int main(void)
{
    // 배열의 복사본을 저장하는 리스트. (원소의 크기가 sizeof(int*)인 것에 주의)
    struct list* lst = list_create(sizeof(int*));

    // 참고, 앞서 값을 복사하여 저장할 때에는 배열을 리터럴로서 전달할 수 있었다.
    //       그러나, 본 예제에서는 실제 값이 저장된 변수의 주소가 필요하므로 배열을 실제로 할당해야 한다.
    int a[2] = { 1, 2 };
    int b[2] = { 3, 4 };
    int c[2] = { 5, 6 };

    // 참고, 배열은 변수의 이름을 참조했을 때와 &를 이용해 주소를 참조했을 때 모두 배열의 첫 번째 요소의 주소를 반환한다.
    //       따라서, 배열의 변수 그 자체의 주소를 참조하려면 별도의 포인터를 이용해야 한다.
    int* p = a;
    int* q = b;
    int* r = c;

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
        for (int j = 0; j < 2; j++)
            printf("%d ", temp_int[j]);
        printf("\n");

        free(temp);
    }
    printf("\n");

    // 포인터를 저장하므로 원본을 수정하면 리스트의 요소가 변함.
    // (참고, list_get 연산은 목적지 포인터에 값의 복사본을 전달하므로 리스트의 특정 요소를 수정할 수 없음)
    // (본 예제는 포인터를 전달하므로 포인터가 가리키는 '변수'의 값을 변경하는 것은 가능하나 포인터 그 자체를 변경하여 다른 변수를 가리키도록 할 수는 없음)
    c[0] = 500;

    printf("원본 수정 후 리스트: \n");
    for (size_t i = 0; i < lst->size; i++)
    {
        void* temp = malloc(sizeof(int*));

        list_get(lst, temp, i);

        int* temp_int = *(void**)temp;
        for (int j = 0; j < 2; j++)
            printf("%d ", temp_int[j]);
        printf("\n");

        free(temp);
    }
    printf("\n");

    list_remove(lst);

    return 0;
}