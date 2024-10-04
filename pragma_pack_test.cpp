#include <iostream>

#pragma pack(4)
struct A{
    char ch;
    double a;
    char c;
};

int main(){
   
    std::cout << sizeof(struct A) << std::endl;
    return 0;
} 