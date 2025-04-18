#include <iostream>
#include <thread>
#include <shared_mutex>
#include <vector>
#include <string>
#include <chrono>

class ThreadSafeCounter
{
private:
    mutable std::shared_mutex mutex_;
    int value_ = 0;

public:
    // 多个线程可以同时读取计数器的值
    int get() const
    {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return value_;
    }

    // 只有一个线程可以递增计数器
    void increment()
    {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        ++value_;
    }

    // 只有一个线程可以重置计数器
    void reset()
    {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        value_ = 0;
    }
};

int main()
{
    ThreadSafeCounter counter;

    auto reader = [&counter](int id)
    {
        for (int i = 0; i < 3; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "Reader " << id << " sees value " << counter.get() << std::endl;
        }
    };

    auto writer = [&counter](int id)
    {
        for (int i = 0; i < 3; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            counter.increment();
            std::cout << "Writer " << id << " incremented value to " << counter.get() << std::endl;
        }
    };

    std::vector<std::thread> threads;

    // 启动3个读取线程
    for (int i = 0; i < 3; ++i)
    {
        threads.push_back(std::thread(reader, i));
    }

    // 启动2个写入线程
    for (int i = 0; i < 2; ++i)
    {
        threads.push_back(std::thread(writer, i));
    }

    // 等待所有线程完成
    for (auto& t : threads)
    {
        t.join();
    }

    return 0;
}
