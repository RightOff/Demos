#include <iostream>
#include <tuple>
#include <functional>

int main(){
    std::tuple<int, char, double> mytuple(10, 'a',3.14);
    std::cout << std::tuple_size<decltype(mytuple)>::value << std::endl;
    std::cout << std::get<0>(mytuple) << std::endl;
    std::tuple_element<1,decltype(mytuple)>::type name;



    std::tuple_element<0, decltype(mytuple)>::type age;
    std::tuple_element<2, decltype(mytuple)>::type scores;
    // std::tie(age, name, scores) = mytuple;
    std::tie(age, name, std::ignore) = mytuple;

    std::cout << "Output: " << '\n';
    std::cout << "name: " << name <<", ";
    std::cout << "age: " << age << ", ";
    std::cout << "scores: " << scores << ", " << std::endl;

    std::cout << typeid(name).name() << std::endl;
    std::cout << typeid(age).name() << std::endl;
    std::cout << typeid(scores).name() << std::endl;

    
    int myage;
    char mychar;
    double mydouble;
    auto mytuple2 = std::make_tuple(std::ref(myage), std::ref(mychar), std::ref(mydouble)) = mytuple;
    std::cout << "Output: " << '\n';
    std::cout << "mychar: " << mychar <<", ";
    std::cout << "myage: " << myage << ", ";
    std::cout << "mydouble: " << mydouble << ", " << std::endl;

    myage = 33;
    mychar = 'g';
    mydouble = 4.23;
    std::cout << "Output: " << '\n';
    std::cout << "mychar: " << mychar <<", ";
    std::cout << "myage: " << myage << ", ";
    std::cout << "mydouble: " << mydouble << ", " << std::endl;




    return 0;
}