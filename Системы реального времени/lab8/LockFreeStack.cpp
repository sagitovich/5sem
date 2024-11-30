#include <iostream>
#include <thread>
#include <vector>

#include "node.hpp"
#include "lockfree_stack.hpp"

namespace {
    // typedef int lf::Position;  // комментирование этого под огромным вопросом
    size_t readers_num = 4;
    lf::LockFreeVersionedStack<lf::Position> stack(readers_num);

    // СТАРЫЙ writer() - ИЗ ЛЕКЦИИ
    /*
    void writer() {
        for (size_t i = 0; i < 10000; i++)
        {
            for (int i = 0; i < 1000; i++)
            {
                stack.push(i);
            }
            for (int i = 0; i < 1000; i++)
            {
                if (!stack.pop()) {
                    throw std::runtime_error("can't delete element");
                }
            }
            for (int i = 0; i < 500; i++)
            {
                stack.push(i);
            }
            for (int i = 0; i < 500; i++)
            {
                if (!stack.pop()) {
                    throw std::runtime_error("can't delete element");
                }
            }
            //Sleep(1);
        }
        stack.stop();
        std::cout << "All versions" << stack.last_version() << std::endl;
    }
    */

    // НОВЫЙ writer() - ДЛЯ ВЫПОЛНЕНИЯ ЛАБОРОТОРНОЙ
    void writer() {
        for (const auto &pos: lf::generate_positions(0.0, 4.0, 0.1)) {
            stack.push(pos);
        }
        stack.stop();
    }

    class Reader {
    public:
        Reader(const unsigned int id, lf::LockFreeVersionedStack<lf::Position>* stack) : id_(id), stack_(stack) {}

        void life() {
            while (!stack_->is_stopped()) {
                auto data = read();
                check(data, 0.1);
            }
        }

        std::vector<lf::Position> read() {
            lf::LockFreeVersionedStack<lf::Position>::NodePtr data_ptr;
            if (!stack_->subscribe(id_, data_ptr)) {
                return {}; // Если подписка не удалась, возвращаем пустой вектор
            }

            std::vector<lf::Position> result;
            while (data_ptr != nullptr) {
                result.push_back(data_ptr->data); // Добавляем данные в вектор
                data_ptr = data_ptr->next;
            }
            stack_->unsubscribe(id_);
            versions_cnt++;
            return result; // Возвращаем заполненный вектор
        }

        // СТАРЫЙ check() - ИЗ ЛЕКЦИИ
        /*
        static void check(const std::vector<Data>& data) {
            if (data.size() > 1000) {
                throw std::logic_error("Wrong sequence size");
            }
            if (data.size() > 1) {
                for (size_t i = 0; i < data.size() - 1; i++)
                {
                    if (data[i] - data[i - 1] != -1) {
                        throw std::logic_error("Wrong sequence");
                    }
                }
            }
        }
        */


        // НОВЫЙ check() - ДЛЯ ВЫПОЛНЕНИЯ ЛАБОРОТОРНОЙ
        static void check(const std::vector<lf::Position>& data, const double step) {
            for (size_t i = 1; i < data.size(); ++i) {
                if (const double calculated_y = -(data[i].x * data[i].x) + 4 * data[i].x;
                    std::abs(calculated_y - data[i].y) > 1e-6) {
                    throw std::logic_error("Точка не принадлежит уравнению");
                }
                if (std::abs(data[i - 1].x - data[i].x - step) > 1e-6) {
                    throw std::logic_error("Нестабильный шаг между точками");
                }
            }
        }

        unsigned int versions_cnt = 0;
    private:
        int id_;
        lf::LockFreeVersionedStack<lf::Position> *stack_;
        // std::thread thread_;  // комментирование этого под вопросом
    };

}

int main() {
    // Инициализация читателей и потоков
    std::vector<Reader> readers;
    std::vector<std::thread> threads;
    readers.reserve(readers_num);
    threads.reserve(readers_num);

    for (int i = 0; i < readers_num; i++) {
        readers.emplace_back(i, &stack);
    }

    for (int i = 0; i < readers_num; i++) {
        threads.emplace_back(&Reader::life, &readers[i]);
    }

    // Запуск писателя
    writer();

    // Завершаем потоки читателей
    for (auto &t : threads) {
        t.join();
    }

    // Итоговая информация
    for (size_t i = 0; i < readers_num; i++) {
        std::cout << "Читающий поток: " << i + 1 << " | Версий прочитано: " << readers[i].versions_cnt << std::endl;
    }

    std::cout << "Тест успешно завершён!" << std::endl;
    return 0;
}
