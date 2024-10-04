#include <iostream>
 
class B;  // 前向声明
 
class A 
{
public:
    B* b;  // A 持有 B 的指针
    ~A() { std::cout << "A destroyed\n"; }
};
 
class B 
{
public:
    A* a;  // B 持有 A 的指针
    ~B() { std::cout << "B destroyed\n"; }
};
 
void createCycle() 
{
    A* a = new A();
    B* b = new B();
    a->b = b;  // A 持有 B
    b->a = a;  // B 持有 A
 
    // 此时 a 和 b 都不会被释放，导致内存泄漏
}
 
int main() 
{
    createCycle();
    return 0;
}