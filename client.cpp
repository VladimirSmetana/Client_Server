#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    const char *server_ip = "127.0.0.1"; // IP-адрес сервера
    int port = 3000;

    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Преобразование IP-адреса с помощью inet_pton
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Неверный адрес/адрес не поддерживается" << std::endl;
        return -1;
    }

    // Подключение к серверу
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Ошибка подключения к серверу" << std::endl;
        return -1;
    }

    // Чтение сообщения от сервера
    read(sock, buffer, sizeof(buffer));
    std::cout << "Сообщение от сервера: " << buffer << std::endl;

    close(sock); // Закрываем сокет
    return 0;
}
