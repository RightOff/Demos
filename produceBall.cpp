#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

std::condition_variable con;
std::mutex mutex;
int red_ball_count = 0; //已经生产的红球
int blue_ball_count = 0; //已经生产的蓝球

const int red_ball_limit = 2;   //每组红球最大数量
const int blue_ball_limit = 1;   //每组蓝球最大数量

void resetCount(){
    if(red_ball_count == red_ball_limit && blue_ball_count == blue_ball_limit){
        red_ball_count = 0;
        blue_ball_count = 0;
    }
    return;
}

void produceRed(){
    while(1){
        std::unique_lock<std::mutex> lock(mutex);
        con.wait(lock, []{ return red_ball_count < red_ball_limit;});

        std::cout << "红球" << std::flush;
        red_ball_count++;
        
        resetCount();

        con.notify_all();
    }

}

void produceBlue(){
    while(1){
        std::unique_lock<std::mutex> lock(mutex);
        con.wait(lock, []{ return blue_ball_count < blue_ball_limit;});

        std::cout << "蓝球" << std::flush;
        blue_ball_count++;

        resetCount();

        con.notify_all();
    }
}



int main(){
    std::thread thr1(produceRed);
    std::thread thr2(produceBlue);

    thr1.join();
    thr2.join();

    return 0;
}