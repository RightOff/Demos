#include <iostream>
#include <functional>   //std::funcion的头文件

class Base{
public:
    int a;
    int b;

    void display(){
        std::cout << "Base display" << std::endl;
    }

    static void count(){
        std::cout << "Base Count" << std::endl;
    }
};
class Func{
public:
    void operator()(){
        std::cout << "func functor" << std::endl;
    }
};


void func1(std::function<void(void)> f){
    std::cout << "func1" << std::endl;
    f();
}
void func2(std::function<void(void)>& f){
    std::cout << "func2" << std::endl;
    f();
}

void func3(void(*f)()){
    std::cout << "func3" << std::endl;
    f();
}
void func(){
    std::cout << "func" << std::endl;
}


int main(){

    //————————————————————函数指针————————————————————
    //函数指针缺点：
    //  不支持指向仿函数
    //  指向成员函数时，需要定义“类名::*”类型的指针，且调用时也需要借助对象进行调用

    void (Base::*p)() = &Base::display; //需要加&取地址
    Base base;
    (base.*p)();    //注意要先解引用函数指针

    void (*p2)() = Base::count; //指针不需要指定类名，且不需要加&取地址
    p2();

    //————————————————————std::funcition————————————————————

    //类成员函数
    std::function<void(void)> p3 = std::bind(&Base::display, &base);
    p3();

    //类静态成员函数
    p3 = Base::count;
    p3();

    //functor仿函数
    p3 = Func();    //要加括号
    p3();

    //lambda
    p3 = [](){
        std::cout << "lambda" << std::endl;
    };
    p3();

    ///————————————————————分别以参数形式传递————————————————————

    func1(func);    //func隐式转换为std::function

    // func2(func); //形参是std::function的引用，故而报错
    std::function<void(void)> p4 = func;
    func2(p4);  //显式传入

    func3(func);



    return 0;
}