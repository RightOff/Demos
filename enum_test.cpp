#include <iostream>

enum A{
    C = 1,
    B = 3,
    D,
    E = 8
};

enum class AA{
    C = 10,
    B = 13,
    D,
    E = 18
};

int main(){
    A a = D;
    AA aa = AA::D;  //必须用作用域，不然默认为不限作用域的A，会报错

    A a2 = A::E;    
    AA aa2 = AA::E; //必须用作用域，不然默认为不限作用域的A，会报错

    
    std::cout << a << std::endl;
    std::cout << int(aa) << std::endl;
    std::cout << a2 << std::endl;
    std::cout << int(aa2) << std::endl;




    return 0;
} 