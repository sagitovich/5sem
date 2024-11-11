#include <iostream>
#include <thread>
#include <atomic>
#include <string>

class MyFavoriteMutex {

    std::atomic<bool> locked{false};

    public:
        void lock() {
            bool flag = false;
            while (!locked.compare_exchange_strong(flag, true, std::memory_order_acquire)) {
                flag = false;
            }
        }

        void unlock() {
            locked.store(false, std::memory_order_release);
        }
};

int coins = 11;            // общее количество монет
bool flag = false;
int Bob_coins = 0;         // монеты Боба
int Tom_coins = 0;         // монеты Тома
MyFavoriteMutex mudex;     // заменяем std::mutex на CustomMutex

void coin_sharing(const std::string& name, int& thief_coins, const int& companion_coins) {
    while (true) {
        mudex.lock();

        if (coins == 1 && Bob_coins == Tom_coins) {
            coins--;
            std::cout << "Последняя монета достается покойнику. Осталось монет: " << coins << std::endl;
            mudex.unlock();
            break;
        }

        if (coins > 0 && thief_coins <= companion_coins) {
            thief_coins++;
            coins--;
            std::cout << name << " взял монету. У него теперь "
                      << thief_coins << " монет. Осталось монет: "
                      << coins << std::endl;
            mudex.unlock();
        } else {
            mudex.unlock();
            if (coins == 0) {
                break;
            }
        }
    }
}

int main() {
    if (coins % 2 != 0) {
        flag = true;
    }

    std::thread bob_thread(coin_sharing, "Боб", std::ref(Bob_coins), std::ref(Tom_coins));
    std::thread tom_thread(coin_sharing, "Том", std::ref(Tom_coins), std::ref(Bob_coins));

    bob_thread.join();
    tom_thread.join();

    std::cout << "Итог:\n"
              << "Монет у Боба: " << Bob_coins << "\n"
              << "Монет у Тома: " << Tom_coins << "\n"
              << "Всего монет у воров: " << (Bob_coins + Tom_coins) << "\n";
    if (flag) {
        std::cout << "Покойник получил последнюю монету." << std::endl;
    }

    return 0;
}
