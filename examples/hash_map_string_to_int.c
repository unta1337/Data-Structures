#include <stdio.h>
#include <string.h>

#include "../hash_map.h"

// 문자열을 일차적으로 해싱한 후 이를 다시 해싱.
// Ref: '김상진', "한국기술교육대학교", 자료구조및실습.
size_t hash_size_t(size_t x)
{
    size_t a = 429127;
    size_t b = 672199;
    size_t P = 278289879;

    return (a * x + b) % P;
}

// 문자열을 일차적으로 해싱.
size_t hash_string(const void* p)
{
    char* q = *(char**)p;
    size_t h = 0;
    size_t n = strlen(q);

    for (size_t i = 0; i < n ; i++)
        h += q[i] * 27 << (n - i - 1);

    return hash_size_t(h);
}

// 문자열을 비교하는 함수
int comp(const void* p, const void* q)
{
    // p와 q가 가리키는 주소는 키 문자열 리터럴을 가리키는 포인터이므로 이중 포인터로 처리한다.
    return strcmp(*(char**)p, *(char**)q);
}

int main(void)
{
    struct hash_map* hash_map = hash_map_create(sizeof(char*), sizeof(int), hash_string, comp);

    char* key = "This should be 10.";           // 문자열 저장 시 이중 포인터를 활용해 문자열 리터럴을 저장할 수 있다.
    int value = 10;                             // 값으로 사용할 정수 변수.
    hash_map_push(hash_map, &key, &value);      // 키와 값을 주소를 유지하는 포인터를 넘겨준다.
    printf("(\"%s\", %d) has been pushed.\n", key, value);

    key = "This should be 20.";
    value = 20;
    hash_map_push(hash_map, &key, &value);
    printf("(\"%s\", %d) has been pushed.\n", key, value);

    key = "올해의 연도는";                      // 한글 또한 유니코드의 바이트 단위로 저장되므로 적절한 해시 함수를 정의하여 삽입할 수 있다.
    value = 2022;
    hash_map_push(hash_map, &key, &value);
    printf("(\"%s\", %d) has been pushed.\n\n", key, value);

    key = "This should be 10.";
    hash_map_get(hash_map, &value, &key);       // 요소를 찾을 때나 삭제할 때도 키와 값의 포인터를 넘겨준다.
    printf("%s And the value is %d.\n", key, value);

    key = "This should be 20.";
    hash_map_get(hash_map, &value, &key);
    printf("%s And the value is %d.\n", key, value);

    key = "올해의 연도는";
    hash_map_get(hash_map, &value, &key);
    printf("%s %d년.\n", key, value);

    hash_map_delete(hash_map);

    printf("\nCurrent Memory Usage (should be 0): %zu.\n", used_malloc);

    return 0;
}
