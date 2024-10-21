#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        std::cerr << "Использование: " << argv[0] << " <имя> <порт>" << std::endl;
        return -1;
    }

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    const char *server_ip = "127.0.0.1"; // IP-адрес сервера
    int port = atoi(argv[2]);

    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Преобразование IP-адреса с помощью inet_pton
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Неверный адрес/адрес не поддерживается" << std::endl;
        return -1;
    }

    // Подключение к серверу
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Ошибка подключения к серверу" << std::endl;
        return -1;
    }

    std::ofstream file("log.txt"); // открываем файл для записи
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл для записи" << std::endl;
        close(sock);
        return -1;
    }

    // Чтение сообщения от сервера
    while (true)
    {
        ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
        
        if (bytes_read < 0) {
            std::cerr << "Ошибка чтения из сокета" << std::endl;
            break; // Выход из цикла при ошибке
        }
        else if (bytes_read == 0) {
            std::cout << "Соединение закрыто сервером" << std::endl;
            break; // Выход из цикла при закрытии соединения
        }

        buffer[bytes_read] = '0'; // Завершаем строку нулевым символом
        std::cout << " [" << buffer << "] " << argv[1] << std::endl;
        file << " [" << buffer << "] " << argv[1] << std::endl; // Запись в файл
    }

    file.close(); // Закрываем файл
    close(sock); // Закрываем сокет
    return 0;
}
