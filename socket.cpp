#include "socket.h"

int Socket::create_and_bind_socket()
{
    struct sockaddr_in address;
    int opt = 1;

    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Установка параметров сокета
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Слушаем на всех интерфейсах
    address.sin_port = htons(port);

    // Привязка сокета к порту
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

void Socket::listen_connect()
{
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "Listening... " << std::endl;
    }
}

int Socket::accept_socket()
{
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (new_socket < 0)
    {
        perror("accept");
    }

    std::cout << "Подключение установлено!" << std::endl;
    return new_socket;
}
int server_fd;
int Socket::init_socket()
{
    int server_fd = create_and_bind_socket();
    listen_connect();
    return server_fd;
}
