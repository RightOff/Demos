#include <memory>
#include <iostream>

template<class T>
class smart_ptr{
    smart_ptr(T* ptr)
        :_ptr(ptr)
    {}
    ~smart_ptr(){
        if(_ptr){
            delete _ptr;
            _ptr = nullptr;
        }
    }

    T& operator*() const{
        return *_ptr;
    }
    T* operator->() const{  //T* = const T* _ptr    ??
        return _ptr;
    }
    T* get() const{
        return _ptr;
    }
private:
    T* _ptr;
};

struct Node{
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> pre;
    ~Node(){
        std::cout << "Node destroyed" << std::endl;
    }
};


int main(){
    {
        std::cout << "--------------------------------unique_ptr--------------------------------" <<std::endl;
        std::unique_ptr<int> ptr1 = std::make_unique<int>(10);

        // std::unique_ptr<int> ptr2 = ptr1;   //试图直接赋值会报错，因为unique_ptr不可复制
        std::unique_ptr<int> ptr2 = std::move(ptr1);
        
        std::unique_ptr<int[]> arrPtr1(new int[5]);
    
        for(int i = 0; i < 5; i++){
            arrPtr1[i] = i;
        }
    }

    {    
        std::cout << "--------------------------------shared_ptr--------------------------------" <<std::endl;
        std::shared_ptr<int> ptr3 = std::make_shared<int>(10);
        std::shared_ptr<int> ptr4(new int(10));

        std::shared_ptr<int> ptr5 = ptr3;

        std::cout << ptr3.use_count() <<std::endl;
    }


    {
        std::cout << "--------------------------------循环引用解决：weak_Ptr--------------------------------" <<std::endl;
        std::shared_ptr<Node> node1(new Node());
        std::shared_ptr<Node> node2(new Node());
        node1->next = node2;
        node2->pre = node1;

        std::shared_ptr<int> ptr6(new int(500));
        std::weak_ptr<int> weakPtr = ptr6;
        if(std::shared_ptr<int> lockedPtr = weakPtr.lock()){   //尝试提升为 shared_ptr
            std::cout << "Value: " << *lockedPtr << std::endl;
        }
        else{
            std::cout << "Pointer is expired" << std::endl;
        }

        ptr6.reset();   //手动释放shared_ptr 所管理的对象
        if(weakPtr.expired()){
            std::cout << "Pointer is expired" << std::endl; // 输出 Pointer is expired
        }
    }
    return 0;


}