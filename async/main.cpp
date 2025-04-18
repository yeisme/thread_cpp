#include <iostream>
#include <future>
#include <thread>
#include <chrono>

// 返回int类型的简单函数
int calculate(int a, int b)
{
    std::cout << "计算线程ID: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); // 模拟耗时操作
    return a + b;
}

// 可能抛出异常的函数
double divide(double a, double b)
{
    if (b == 0.0)
    {
        throw std::runtime_error("除数不能为零");
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return a / b;
}

int main()
{
    std::cout << "主线程ID: " << std::this_thread::get_id() << std::endl;

    // 1. 默认启动策略(根据系统决定是否真正创建线程)
    std::future<int> result1 = std::async(calculate, 10, 20);

    // 2. 指定立即异步执行
    std::future<int> result2 = std::async(std::launch::async, calculate, 30, 40);

    // 3. 指定延迟执行(调用get或wait时才执行)
    std::future<int> result3 = std::async(std::launch::deferred, calculate, 50, 60);

    std::cout << "异步任务已启动..." << std::endl;

    // 获取结果1(阻塞等待完成)
    std::cout << "结果1: " << result1.get() << std::endl;

    // 检查result2是否已准备好
    if (result2.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
    {
        std::cout << "结果2已准备好" << std::endl;
    }
    std::cout << "结果2: " << result2.get() << std::endl;

    // 延迟任务在调用get时才执行
    std::cout << "开始获取结果3(将触发延迟任务执行)" << std::endl;
    std::cout << "结果3: " << result3.get() << std::endl;

    // 异常处理示例
    try
    {
        auto result4 = std::async(divide, 10.0, 0.0);
        double value = result4.get(); // 异常会在这里传播到主线程
    }
    catch (const std::exception& e)
    {
        std::cout << "捕获异常: " << e.what() << std::endl;
    }

    // 带lambda表达式的异步任务
    auto result5 = std::async([](const int x)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return x * x;
    }, 8);

    std::cout << "结果5: " << result5.get() << std::endl;

    return 0;
}
