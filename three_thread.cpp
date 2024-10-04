#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
int current_thread = 1;  // 1 表示线程 1 打印A，2 表示线程 2 打印B，3 表示线程 3 打印C

void printA() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return current_thread == 1; });
        std::cout << 'A' << std::flush; //立即刷新
        current_thread = 2;  // 设置下一个要打印的线程
        cv.notify_all();  // 通知其他线程
    }
}

void printB() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return current_thread == 2; });
        std::cout << 'B' << std::flush;
        current_thread = 3;  // 设置下一个要打印的线程
        cv.notify_all();  // 通知其他线程
    }
}

void printC() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return current_thread == 3; });
        std::cout << 'C' << std::flush; 
        current_thread = 1;  // 设置下一个要打印的线程
        cv.notify_all();  // 通知其他线程
    }
}

int main() {
    
    std::thread t1(printA);
    std::thread t2(printB);
    std::thread t3(printC);

    
    t1.join();
    t2.join();
    t3.join();

    return 0;
}