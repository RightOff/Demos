#include <iostream>
#include <future>
#include <chrono>
std::string fetchDataFromDB(std::string query){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return "Data: " + query;
}

int main(){
    //使用async异步调用fetchDataFromDB
    std::future<std::string> resultFromDB = std::async(std::launch::async, fetchDataFromDB, "data");
    
    std::cout << "Doing something else..." << std::endl;

    //从future中获取数据
    std::string dbData = resultFromDB.get();
    std::cout << dbData << std::endl;


    return 0;
}
