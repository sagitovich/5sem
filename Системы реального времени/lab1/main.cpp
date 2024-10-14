#include <iostream>
#include <thread>
#include <iomanip>

using namespace std;

void factorial() {
    for (int k = 0; k < 10000000; k++)
    {
        constexpr int n = 65;
        unsigned long long result = 1;
        for (int i = 1; i <= n; ++i) {
            result *= i;
        }
    }
}

int main() {
    setlocale(LC_ALL, "ru");

    // параллельно
    const clock_t start_parallel = clock();

    thread thread1(factorial);
    thread thread2(factorial);
    thread1.join();
    thread2.join();

    const clock_t end_parallel = clock();
    const double result_parallel = static_cast<double>(end_parallel - start_parallel) / CLOCKS_PER_SEC;
    cout << "Parallel: " << result_parallel << " sec" << endl;

    // последовательно
    const clock_t start_sequential = clock();

    factorial();
    factorial();

    const clock_t end_sequential = clock();
    const double result_sequential = static_cast<double>(end_sequential - start_sequential) / CLOCKS_PER_SEC;
    cout << std::fixed << std::setprecision(5);
    cout << "Sequential: " << result_sequential << " sec";

    return 0;
}
