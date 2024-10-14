#include <iostream>
#include <thread>
#include <string>
#include <chrono>

std::mutex m; // global mutex

void Func(std::string name)
{
    long double i = 0;
    const auto start = std::chrono::high_resolution_clock::now();

    // в течение одной секунды
    while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start).count() < 1) {
        i += 1e-9;
    }

    // using mutex
    m.lock();
    std::cout << name << ": " << i << std::endl;
    m.unlock();
}

int main()
{
    std::thread thread1(Func, "t1");
    std::thread thread2(Func, "t2");
    std::thread thread3(Func, "t3");

    thread1.join();
    thread2.join();
    thread3.join();

    system("pause");
    return 0;
}


