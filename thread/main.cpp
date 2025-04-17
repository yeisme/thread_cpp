#include <iostream>
#include <thread>
#include <vector>
#include <future>

void calculate_sum(const std::vector<int>& v, int begin, int end, std::promise<int> promise)
{
    int sum = 0;
    for (int i = begin; i < end; ++i)
    {
        sum += v[i];
    }
    promise.set_value(sum);
}

int main()
{
    std::vector<int> v(10000, 1);
    std::promise<int> promise1, promise2;
    auto future1 = promise1.get_future();
    auto future2 = promise2.get_future();

    std::thread t1(calculate_sum, std::ref(v), 0, 5000, std::move(promise1));
    std::thread t2(calculate_sum, std::ref(v), 5000, 10000, std::move(promise2));

    t1.join();
    t2.join();

    std::cout << "Sum: " << future1.get() + future2.get() << std::endl;
    return 0;
}
