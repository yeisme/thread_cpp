//
// Created by yeisme on 25-4-20.
//
#include <iostream>
#include <thread>
#include <mutex>

class recursive_example
{
private:
    std::recursive_mutex resource_mutex_;
    int resource_value_ = 0;

public:
    void method1()
    {
        std::lock_guard lock(resource_mutex_);
        std::cout << "method1 �����, ��������: " << resource_value_ << std::endl;
        resource_value_++;
        method2();
    }

    void method2()
    {
        std::lock_guard lock(resource_mutex_); // �ٴ�����
        std::cout << "method2 �����, ��������: " << resource_value_ << std::endl;
        resource_value_++;
    }

    int get_value()
    {
        std::lock_guard lock(resource_mutex_);
        return resource_value_;
    }
};

auto main() -> int
{
    recursive_example example;
    std::thread t([&example]()
    {
        example.method1();
    });
    t.join();
    std::cout << "����ֵ: " << example.get_value() << std::endl;
    return 0;
}
