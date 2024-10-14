// #include <iostream>
// #include <conio.h>
// #include <stdio.h>
// #include <ctime>

// int main() {
//     clock_t start, end;
//     start = clock();
//     std::cout << "Press any key to start\n";
    
//     char x = _getch();  // Ожидание начала
//     std::cout << "Start - " << x << std::endl;

//     std::string message = "The program is still alive!";  // Начальное сообщение

//     while (true) {
//         end = clock();
//         if ((double)(end - start) / CLOCKS_PER_SEC >= 1) {  // Прошла ли секунда?
//             std::cout << message << "\n";
//             start = clock();
//         }

//         if (_kbhit()) {  // Проверка нажатия клавиши
//             char key = _getch();
//             if (key == 'q') {  // Выход при нажатии 'q'
//                 std::cout << "Exiting program...\n";
//                 break;
//             } else {
//                 message = "You pressed: ";
//                 message += key;  // Обновление сообщения
//             }
//         }
//     }

//     return 0;
// }
// #include <conio.h>
// #include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <ctime>

// Функция для отключения буферизации и эха для терминала
void set_nonblock_terminal(bool enable) {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (enable) {
        tty.c_lflag &= ~(ICANON | ECHO);  // Отключаем канонический режим и вывод эха
    } else {
        tty.c_lflag |= (ICANON | ECHO);   // Включаем канонический режим и вывод эха
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

// Аналог _kbhit() для Linux/macOS
bool kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0;
}

// Аналог _getch() для Linux/macOS
char getch() {
    char buf = 0;
    read(STDIN_FILENO, &buf, 1);
    return buf;
}

int main() {
    clock_t start, end;
    start = clock();
    
    set_nonblock_terminal(true);  // Отключаем буферизацию ввода
    
    std::cout << "Нажми любую клавишу для начала\n";
    
    while (!kbhit()) {
        // Ждём нажатия клавиши
    }
    
    char x = getch();  // Ожидание клавиши для старта
    std::cout << "Start - " << x << std::endl;

    std::string message = "Программа выполняется...";

    while (true) {
        end = clock();
        if ((double)(end - start) / CLOCKS_PER_SEC >= 1) {
            std::cout << message << "\n";
            start = clock();
        }

        if (kbhit()) {  // Проверка нажатия клавиши
            char key = getch();
            if (key == 'q') {  // Выход при нажатии 'q'
                std::cout << "Завершение работы программы\n";
                break;
            } else {
                message = "Нажата клавиша: ";
                message += key;  // Обновление сообщения
            }
        }
    }

    set_nonblock_terminal(false);  // Возвращаем настройки терминала

    return 0;
}