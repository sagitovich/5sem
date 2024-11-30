#pragma once

namespace lf {
    template<typename T>
    struct Node {
        typedef Node<T>* NodePtr;

        T data;
        NodePtr next;
        uint64_t version;
    };
}

// У шаблона с оператором пеш есть особенности, если объекту требуются аргументы.
// Без allocator object или пляками с преобразованием указателей и размером выделяемой памяти
// // этого не сделать. Выглядит громоздко, оставим пример для объектов с дефолтным конструктором.