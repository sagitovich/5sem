#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const char* SERVER_IP = "127.0.0.1";
const int PORT = 8080;

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Создаем сокет
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Преобразование IP-адреса
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Неверный / неподдерживаемый IP-адрес\n";
        return 1;
    }

    // Подключение к серверу
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Ошибка подключения\n";
        return 1;
    }

    std::cout << "Подключение к серверу прошло успешно.\n";

    // Основной цикл отправки сообщений
    while (true) {
        std::cout << "Введите сообщение: ";
        std::string message;
        std::getline(std::cin, message);

        // Отправка сообщения серверу
        send(sock, message.c_str(), message.size(), 0);

        // Условие выхода
        if (message == "камеру вырубай") {
            std::cout << "Выход...\n";
            break;
        }
    }

    close(sock);
    return 0;
}