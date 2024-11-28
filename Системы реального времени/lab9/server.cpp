#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

const int PORT = 8080;

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    char buffer[1024] = {0};

    // Создаем сокет
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Ошибка создания токена");
        return 1;
    }

    // Указываем адрес и порт
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Связываем сокет с портом
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Ошибка бинда");
        return 1;
    }

    // Прослушивание входящих соединений
    if (listen(server_fd, 3) < 0) {
        perror("Ошибка прослушивания");
        return 1;
    }

    std::cout << "Сервер запущен. Порт: " << PORT << "...\n";

    // Принимаем соединение
    socklen_t addrlen = sizeof(address);
    if ((client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
        perror("Ошибка принятия");
        return 1;
    }

    std::cout << "Клиент успешно подключился к серверу!\n";

    // Основной цикл обработки сообщений
    while (true) {
        memset(buffer, 0, sizeof(buffer));  // Очищаем буфер
        int valread = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

        if (valread < 0) {
            perror("Ошибка получения");
            break;
        } else if (valread == 0) {
            std::cout << "Клиент отключился от сервера.\n";
            break;
        }

        // Отображаем сообщение от клиента
        std::cout << "Сообщения клиента: " << buffer << std::endl;

        // Условие выхода
        if (strcmp(buffer, "камеру вырубай\n") == 0) {
            std::cout << "Ладно-ладно, вырубаю (сервер отключается)...\n";
            break;
        }
    }

    close(client_socket);
    close(server_fd);
    return 0;
}