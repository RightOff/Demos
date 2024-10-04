
#include <iostream>

#include <vector>

int main(){

    std::vector<int> nums(10,2);

    for(int i = 0; i < nums.size(); i++){ //小于等于会导致运行时报错
        nums[i] = i;    
    }

    
    for(int i = 0; i < nums.size(); i++){ 
        std::cout << nums[i] << std::endl;
    }



    return 0;
}