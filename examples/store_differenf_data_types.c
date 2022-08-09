#include <stdio.h>

#include "../list.h"

int main(void)
{
    // 다수의 자료형을 저장하므로 포인터를 저장해야 한다.
    // 그런데 자료형을 특정하지 않으므로 void*의 크기를 이용해 리스트를 생성한다.
    struct list* lst = list_create(sizeof(void*));

    // int, float, double, char* 자료형의 요소들.
    int int_elem = 42;
    float float_elem = 3.14f;
    double double_elem = 2.71;
    char* string_literal_elem = "Hello, world!";

    // 리스트에 포인터를 삽입, 읽기 위해 사용할 포인터.
    void* temp;

    // 포인터를 삽입할 것이므로 int_elem을 가리키는 포인터를 이용해 해당 포인터의 주소를 넘긴다.
    // 참고, 리스트는 넘겨진 주소를 참조하여 해당 공간에 있는 데이터를 단일 크기만큼 복사해 저장한다.
    //       즉, 역참조했을 때 삽입하기를 원하는 데이터가 나타나는 주소를 넘기면 된다.
    //       이 경우 참조 시 int_elem의 주소가 나타나야 하므로 temp를 그대로 넘겨줬다.
    temp = &int_elem;
    list_push(lst, &temp);

    temp = &float_elem;
    list_push(lst, &temp);

    temp = &double_elem;
    list_push(lst, &temp);

    // 참고, 문자열 리터럴은 이미 그 자체로 포인터이므로 그 주소를 그대로 넘겨줘도 된다.
    //       char* foo = "bar"에서 foo를 역참조하면 문자열 리터럴이 담긴 주소가 된다.
    temp = string_literal_elem;
    list_push(lst, &temp);

    // 리스트로부터 값을 읽기 위해 temp에 공간을 할당.
    // 참고, 삽입과 비슷하게 요소를 읽기 위해서 참조 시 단일 크기만큼의 공간을 가리키는 포인터를 넘지면 된다.
    //       이 경우 temp 역참조 시 sizeof(void*)만큼의 공간이 나타나므로 temp를 그대로 넘겨줬다.
    temp = malloc_s(sizeof(void*));

    // 각 자료형에 맞는 자료형으로 캐스팅 및 역참조햐여 출력.
    list_get(lst, temp, 0);
    printf("%d\n", **(int**)temp);

    list_get(lst, temp, 1);
    printf("%f\n", **(float**)temp);

    list_get(lst, temp, 2);
    printf("%lf\n", **(double**)temp);

    // 문자열 리터럴은 그 자체로 포인터이므로 다른 자료형들과는 다르게 역참조를 한 번만 하여 char*로 만들어준다.
    list_get(lst, temp, 3);
    printf("%s\n", *(char**)temp);

    // 동적할당된 공간 반납.
    free_s(temp);
    list_delete(lst);

    printf("\nCurrent Memory Usage (should be 0): %zu.\n", used_malloc);

    return 0;
}