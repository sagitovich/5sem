#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

constexpr int DISH_CAPACITY = 3000; // начальное кол-во наггетсов в тарелке у каждого толстяка
constexpr int MAX_NUGGETS = 10000;  // число наггетсов, после которых толстяк сделает бум
constexpr int WORK_DURATION = 5;    // кука хватает на 5 дней (секунд)

class Table {
public:
    std::vector<int> dishes;
    std::mutex mtx;

    Table() : dishes(3, DISH_CAPACITY) {}
};

class Fatty {
public:
    Fatty(const int id, Table& table, const int gluttony, std::atomic<int>& exploded_count, std::atomic<bool>& all_fatties_exploded)
        : id(id), table(table), gluttony(gluttony), eaten_nuggets(0), exploded_count(exploded_count), all_fatties_exploded(all_fatties_exploded) {}

    void eat() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ждём перед каждым употреблением еды

            std::lock_guard<std::mutex> lock(table.mtx);
            if (table.dishes[id] > 0) {
                const int to_eat = std::min(gluttony, table.dishes[id]);
                table.dishes[id] -= to_eat;
                eaten_nuggets += to_eat;

                std::cout << "Толстяк " << id + 1 << " съел " << to_eat << " наггетсов, осталось: " << table.dishes[id] << "\n";
                if (eaten_nuggets >= MAX_NUGGETS) {
                    std::cout << "Толстяк " << id + 1 << " самоуничтожился!\n";
                    ++exploded_count;
                    if (exploded_count == 3) {
                        all_fatties_exploded = true;
                    }
                    break;
                }
            } else {
                std::cout << "Толстяк " << id + 1 << " остался без наггетсов! Кук уволен.\n";
                exit(0);
            }
        }
    }

private:
    int id;
    Table& table;
    int gluttony;
    int eaten_nuggets;
    std::atomic<int>& exploded_count;
    std::atomic<bool>& all_fatties_exploded;
};

class Kuk {
    public:
        Kuk(Table& table, const int efficiency, std::atomic<bool>& all_fatties_exploded)
            : table(table), efficiency(efficiency), all_fatties_exploded(all_fatties_exploded) {}

        void cook() const {
            const auto start = std::chrono::steady_clock::now();
            while (true) {
                if (all_fatties_exploded) {
                    std::cout << "Все толстяки самоуничтожились! Кук не получил зарплату.\n";
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // кук готовит каждые 100 мс

                std::lock_guard<std::mutex> lock(table.mtx);
                for (int i = 0; i < table.dishes.size(); ++i) {
                    table.dishes[i] += efficiency;
                    std::cout << "Кук добавил " << efficiency << " наггетсов на тарелку "
                    << i + 1 << ", всего: " << table.dishes[i] << "\n";
                }

                if (auto now = std::chrono::steady_clock::now();
                    std::chrono::duration_cast<std::chrono::seconds>(now - start).count() >= WORK_DURATION)
                {
                    std::cout << "Кук уволился!\n";
                    exit(0);
                }
            }
        }

    private:
        Table& table;
        int efficiency;
        std::atomic<bool>& all_fatties_exploded;
};

int main() {
    Table table;
    constexpr int gluttony = 50;           // прожорливость
    constexpr int efficiency = 500;         // производительность

    std::atomic<int> exploded_count(0);
    std::atomic<bool> all_fatties_exploded(false);

    Fatty fatty1(0, table, gluttony, exploded_count, all_fatties_exploded);
    Fatty fatty2(1, table, gluttony, exploded_count, all_fatties_exploded);
    Fatty fatty3(2, table, gluttony, exploded_count, all_fatties_exploded);
    Kuk cook(table, efficiency, all_fatties_exploded);

    std::thread t1(&Fatty::eat, &fatty1);
    std::thread t2(&Fatty::eat, &fatty2);
    std::thread t3(&Fatty::eat, &fatty3);
    std::thread t4(&Kuk::cook, &cook);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}

// Условие 1: кука уволили
// constexpr int gluttony = 500;
// constexpr int efficiency = 50;

// Условие 2: как не получил з/п
// constexpr int gluttony = 500;
// constexpr int efficiency = 500;

// Условие 3: кук уволился сам
// constexpr int gluttony = 50;
// constexpr int efficiency = 500;
