
//球种类数
#include <iostream>

int main(){
    int count = 0;
    for(int i = 0; i <=3; i++){     //红球个数
        for(int j = 0; j <= 3; j++){    //白球个数
            if(8 - i - j <= 6){
                count++;
            }
        }   
    }
    std::cout << "颜色搭配种数为：" << count << std::endl;
    return 0;
} 