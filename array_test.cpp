
#include <iostream>
#include <climits>

template <int N>
void changeArr(int (&arr)[N][N]){
    arr[3][3] = 100;
}

int main(){
    int n = INT_MAX;
    int arr[10][10];
    std::cout << arr[3][3] << std::endl;
    changeArr<10>(arr);
    std::cout << arr[3][3] << std::endl;



    return 0;
}