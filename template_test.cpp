
#include <iostream>

template <typename T1, typename T2, bool enabled = std::is_base_of<T1, T2>::value>
class Add{
public:
    Add(){
        std::cout << "Add<T1, T2, enabled>" << std::endl;
    }
};

template<typename T1, typename T2>
class Add<T1, T2, false>{
public:
    Add(){
        std::cout << "Add<T1, T2, false>" << std::endl;
    }
};
template<typename T1, typename T2>
class Add<T1, T2, true>{
public:
    Add(){
        std::cout << "Add<T1, T2, true>" << std::endl;
    }
};

class A{

};
class B : public A{

};

int main(){

    Add<A,B> a;
    Add<B,A> b;
    
    Add<int, float> c;

    return 0;
}