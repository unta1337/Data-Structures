# **C를 이용해 구현된 자료구조**  

## **C에서 사용할 수 있는 ADT의 구현**  
* 리스트, 큐, 스택 등, C에서 제공하지 않는 자료구조에 대한 구현이다.  
* 구현하는 ADT와 그 연산은 C++의 STL을 참고했다.  
* 포인터와 동적할당을 활용해 자료구조 구현한다.  
* 개발 단계이므로 개별 함수에 대한 이름이나 매개변수 순서 등은 언제든지 변경될 수 있다.  

## **구현되었거나 구현 예정인 ADT**  
* [x] 페어  
    * [x] 페어  
* [x] 리스트  
    * [x] 리스트  
* [x] 스택  
    * [x] 스택  
* [x] 큐  
    * [x] 큐  
    * [x] 힙큐  
* [x] 덱  
    * [x] 덱  
* [ ] 맵  
    * [ ] 맵  
    * [x] 해시맵  
* [ ] 셋  
    * [ ] 셋  
    * [ ] 해시셋  

## **포인터와 동적할당을 활용한 구현**  
본 리포지토리에 구현된 자료구조는 포인터와 동적할당을 활용해 구현되었다.  
따라서 해당 자료구조를 활용하려면 포인터와 동적할당에 대해 알고 있어야 한다.  

### **포인터**  
포인터는 C와 C++과 같은 언어에서 제공하는 자료형으로서 어떤 변수의 주소 또는 어떤 값이 저장된 메모리상의 주소를 저장한다.  
이때 특정 주소를 저장하고 있는 포인터를 해당 주소를 가리키는 포인터라고 표현한다.  

``` C
int a = 10;
```
위의 코드에서 정수 리터럴 10은 변수 a에 저장되었다.  
그렇다면 실제 메모리상에서 10은 어디에 저장되어 있을까?
이를 확인하려면 앞서 살펴본 포인터를 사용하면 된다.  

``` C
int a = 10;
int* p = &a;
```
위에서 int*는 또 다른 자료형으로 int를 가리키는 포인터를 의미한다.  
& 연산자는 참조 연산자로 피연산자가 저장된 주소를 반환한다. 위의 예시에서는 a를 피연산자로 하므로 a가 저장된 메모리상의 주소를 반환한다.  

``` C
printf("%p\n", p);
```
위의 코드는 포인터의 값을 출력하는 코드로 해당 코드를 실행하면 p에 저장된 값, a의 주소를 출력한다.  
a가 저장된 메모리상의 주소가 0x10이었다면 위의 코드는 0x10을 출력한다.  

``` C
printf("%d\n", *p);
```
포인터가 가리키는 곳에 저장된 값을 확인하려면 위와 같이 역참조 연산자 *를 사용하면 된다.  

``` C
printf("%p\n", &p);
```
한편 포인터도 하나의 변수이므로 이 또한 메모리상의 주소를 갖는다.  
위와 같이 참조 연산자를 사용하면 포인터가 저장된 주소를 알 수 있다.  
이는 본 리포지토리에서 제공하는 자료구조에 포인터를 저장하거나 배열을 저장하는 데 유용하게 쓰일 수 있다.  

### **동적할당**  
동적할당은 힙이라는 메모리에 공간을 마련하는 것으로 추가적인 메모리를 확보할 수 있다.  

stdlib 헤더 파일에 정의된 malloc, realloc 등의 함수를 통해 공간을 확보할 수 있다.  
또 확보된 공간 역시 메모리상의 어딘가에 저장될 것이므로 포인터를 이용해 해당 공간을 활용할 수 있다.  

```  C
#include <stdlib.h>

...

int* p = (int*)malloc(sizeof(int));

...

free(p);
```
위의 코드는 int 자료형 하나 분량의 공간을 할당받는 코드이다.  
마지막 줄의 free 함수를 통해 확보한 공간을 다시 반납하는 것을 볼 수 있다.  
이러한 반납 과정이 없다면 메모리 누수가 발생할 수 있으니 주의해야 한다.  
