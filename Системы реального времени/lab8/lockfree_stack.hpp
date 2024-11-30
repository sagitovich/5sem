#pragma once

#include <iostream>
#include <vector>
#include <atomic>
#include <limits>

#include "node.hpp"

namespace lf {

    // 1 - пишущий поток, несколько читающих
    // Читающие потоки ожидают самую свежую версию
    // Номер версии служит лишь для задачи очистки мусора
    template<typename T>
    class LockFreeVersionedStack {
    public:
        typedef Node<T>* NodePtr;
        typedef std::atomic<Node<T>*> AtomicNodePtr;
        typedef std::atomic_uint AtomicVersion; // полагаем, что переполнение не произойдет
        // версий за время жизни программы меньше чем uint64_t

        explicit LockFreeVersionedStack(const size_t readers_num) : subs_num_(readers_num) {
            stop_flag_.store(false);
            subscribers_ = new AtomicVersion[readers_num];
            for (int i = 0; i < readers_num; i++)
            {
                subscribers_[i].store(0);
            }
        }

        ~LockFreeVersionedStack() {
            while (pop()) {}
            // удаляем все элементы пока указатель не nullptr
            for (auto& node : trash_)
            {
                delete node;
            }
        }

        bool subscribe(const unsigned int& id, NodePtr& stack_ptr) {
            auto& sub = subscribers_[id];
            // Цикл ожидания новой версии
            while (sub.load(std::memory_order_relaxed) == stack_.version.load(std::memory_order_relaxed)) {
                if (stop_flag_) {
                    return false;
                }
            }
            auto unsigned int readed_version = sub.load();
            while (!sub.compare_exchange_strong(readed_version, stack_.version))
                // Повторяем, пока замена не будет успешной
            {
                if (stop_flag_) {
                    return false;
                }
            }
            // Между операцией подписания на версию и получения указателя, поток читателя может прерваться.


            stack_ptr = stack_.head.load();
            return true;
        }

        void unsubscribe(const unsigned int& id) const {
            subscribers_[id].store(0);
        }

        void push(T value) {
            // Только 1 поток создает версии
            // Готовим новую версию
            auto new_node = new Node<T>; // создаем новый элемент
            new_node->data = std::move(value);
            new_node->next = stack_.head.load();
            new_node->version = stack_.version.load() + 1;

            // Заменяем старую на новую версию
            stack_.head.store(new_node);
            // Если между этими строками возникло прерывание
            // читающий поток мог подписаться на новую версию,
            // но stack version ещё не инкрементирована
            // Это не приведет к проблеме, потому что предыдущая версия не успеет удалиться,
            // т.к. она в subscribers[id]|
            // а новый элемент имеет большую версию, чем у head
            ++(stack_.version);
        }

        bool pop() {
            // Только 1 поток создает версии
            // Проверяем, пустой ли список
            NodePtr old_node = stack_.head.load();
            if (old_node == nullptr) {
                return false;
            }
            // Подготавливаем новую версию перед обновлением
            NodePtr new_first_node = old_node->next;
            if (new_first_node != nullptr) {
                // Если это был последний элемент и стек станет пустым,
                // то нужно только инкрементировать версию головы
                new_first_node->version = stack_.version.load() + 1;
            }
            // Обновляем стек
            stack_.head.store(new_first_node);
            ++(stack_.version);

            // Собираем мусор
            update_trash(old_node);
            return true;
        }

        void stop() {
            stop_flag_.store(true);
        }

        bool is_stopped() const {
            return stop_flag_.load();
        }

        unsigned int last_version() {
            return stack_.version.load();
        }
    private:

        // Выполняется в пишущем потоке
        void update_trash(NodePtr old_node) {
            // Добавляем элемент в мусорку
            trash_.push_back(old_node);

            // Ищем наименьшую актуальную версию
            uint64_t min_version = (std::numeric_limits<uint64_t>::max)();
            for (size_t i = 0; i < subs_num_; ++i) {
                // Если читающий поток уже успел подписаться на более новую версию,
                // то минимальная версия для удаления должна стать больше,
                // но это не отменяет того, что найденные версии надо удалять
                // логика удаления не ломается
                const auto version = subscribers_[i].load();
                if (version == 0) {
                    continue;
                }
                min_version = (version < min_version) ? version : min_version;
            }
            // Удаляем все устаревшие ноды
            for (size_t i = 0; i < trash_.size();) {
                if (trash_[i]->version < min_version) { // Проверка условия для удаления
                    delete trash_[i];
                    trash_.erase(trash_.begin() + i); // Удаляем элемент из оригинала и обновляем итератор
                }
                else {
                    ++i; // Переходим к следующему элементу
                }
            }
        }

        struct VersionedHead {
            AtomicVersion version;
            AtomicNodePtr head; // стек пустой, если nullptr
        };

        VersionedHead stack_; // Указатель на начало стека, фиктивный головной элемент
        AtomicVersion* subscribers_; // Список читателей
        // индекс соответствует номеру потока, значение - читаемая версия
        // 0 - поток не подписан
        size_t subs_num_;

        std::vector<NodePtr> trash_; // неупорядоченный контейнер для хранения устаревших элементов
        std::atomic_bool stop_flag_;
    };

    struct Position {
        double x;
        double y;
    };

    inline std::vector<Position> generate_positions(const double start, const double end, const double step) {
        std::vector<Position> positions;
        for (double x = start; x <= end; x += step) {
            positions.push_back({x, -(x * x) + 4 * x});
        }
        return positions;
    }

}
