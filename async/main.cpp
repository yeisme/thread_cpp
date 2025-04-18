#include <iostream>
#include <future>
#include <thread>
#include <chrono>

// ����int���͵ļ򵥺���
int calculate(int a, int b)
{
    std::cout << "�����߳�ID: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); // ģ���ʱ����
    return a + b;
}

// �����׳��쳣�ĺ���
double divide(double a, double b)
{
    if (b == 0.0)
    {
        throw std::runtime_error("��������Ϊ��");
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return a / b;
}

int main()
{
    std::cout << "���߳�ID: " << std::this_thread::get_id() << std::endl;

    // 1. Ĭ����������(����ϵͳ�����Ƿ����������߳�)
    std::future<int> result1 = std::async(calculate, 10, 20);

    // 2. ָ�������첽ִ��
    std::future<int> result2 = std::async(std::launch::async, calculate, 30, 40);

    // 3. ָ���ӳ�ִ��(����get��waitʱ��ִ��)
    std::future<int> result3 = std::async(std::launch::deferred, calculate, 50, 60);

    std::cout << "�첽����������..." << std::endl;

    // ��ȡ���1(�����ȴ����)
    std::cout << "���1: " << result1.get() << std::endl;

    // ���result2�Ƿ���׼����
    if (result2.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
    {
        std::cout << "���2��׼����" << std::endl;
    }
    std::cout << "���2: " << result2.get() << std::endl;

    // �ӳ������ڵ���getʱ��ִ��
    std::cout << "��ʼ��ȡ���3(�������ӳ�����ִ��)" << std::endl;
    std::cout << "���3: " << result3.get() << std::endl;

    // �쳣����ʾ��
    try
    {
        auto result4 = std::async(divide, 10.0, 0.0);
        double value = result4.get(); // �쳣�������ﴫ�������߳�
    }
    catch (const std::exception& e)
    {
        std::cout << "�����쳣: " << e.what() << std::endl;
    }

    // ��lambda���ʽ���첽����
    auto result5 = std::async([](const int x)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return x * x;
    }, 8);

    std::cout << "���5: " << result5.get() << std::endl;

    return 0;
}
