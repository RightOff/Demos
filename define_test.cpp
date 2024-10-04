#include <iostream>

#define f(x) x * x
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

int main(){
    int a = f(3+3) / f(2+2);
    int arr[100];
    std::cout << "aa = " << a << std::endl;
    std::cout << "arr.size = " << ARRAY_SIZE(arr) << std::endl;
    return 0;
} 