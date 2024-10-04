#include <iostream>
#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <functional>
#include <atomic>

class ThreadPool{
public:
    
    // 单例模式
    // 禁用拷贝构造和拷贝赋值
    ThreadPool& operator= (const ThreadPool& t) =delete;
    ThreadPool(const ThreadPool& t ) = delete;

    static ThreadPool& GetThreadPool(int threadNum){
        // static ThreadPool pool(threadNum);  // 懒汉模式
        // return pool;    

        // 要考虑到这里有new 的对象是存放在内存上，当线程池结束的时候会有问题
        // 但这里使用了局部的静态，在c++11以后这样使用是默认线程安全的
        // call_once避免多线程的调用问题
        static ThreadPool* pool = nullptr;
        if(!pool) pool = new ThreadPool(threadNum); // 饿汉模式
        return *pool;
    }

    ThreadPool(int threadNum): stop(false){
        for(int i=0; i<threadNum; i++){
            // 添加线程
            threads.emplace_back([this]{
                while(true){
                    // 加锁，等待加进入任务
                    std::unique_lock<std::mutex> lk(mtx);
                    cv.wait(lk, [this]{return !task_queue.empty()|| stop;});

                    // 线程停止 任务完成
                    if(stop && task_queue.empty()) return;

                    // 取任务
                    std::function<void()> task(std::move(task_queue.front()));
                    task_queue.pop();
                    lk.unlock();
                    task();
                }
            });
        }
    }

    ~ThreadPool(){
        stop = true;
        // 让所有线程取任务，确保任务运行完毕
        cv.notify_all();
        for(auto& t: threads){
            t.join();
        }
    }

    // 函数模板
    // && 右值引用，在这个函数模板下是万能引用（左值 右值）
    // ... 同解构一样
    template<class F, class... Args>
    void addTask(F&& f, Args&&... args){
        // 函数和参数绑定
        // 因为通用引用，通过完美转发更好的绑定
        std::function<void()> task =
            std::bind(std::forward<F>(f), std::forward<Args>(args)...);

        // 加入任务
        {
            std::unique_lock<std::mutex> lk(mtx);
            task_queue.emplace(std::move(task));
        }
        cv.notify_one();
    }

private:
    std::vector<std::thread> threads;               // 线程列表 
    std::queue<std::function<void()>> task_queue;     // 任务队列
    std::condition_variable cv;                     // 条件变量
    std::mutex mtx;     // 互斥锁
    std::atomic<bool> stop;  // 原子操作
};

int main(){
    // 单例模式不可以循环加任务，因为离开for就会析构了
    for(int i=0; i<10; i++){
        std::cout << "add..." << std::endl;
        ThreadPool::GetThreadPool(4).addTask([i]{
            std::cout << std::this_thread::get_id() << " " << i << "running." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << std::this_thread::get_id() << " " << i << "endding." << std::endl;
        });
    }
    // 延迟的对象的析构
    std::this_thread::sleep_for(std::chrono::seconds(15));
    ThreadPool::GetThreadPool(4).~ThreadPool();
    return 0;
}