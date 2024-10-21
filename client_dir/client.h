#include "../socket.h"
#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <mutex>

class Client : public Socket
{
private:
    struct sockaddr_in serv_addr;
    int sock;
    char buffer[1024];
    std::mutex file_mutex;

public:
    explicit Client(const std::string &server_ip, int port) : Socket(port)
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            throw std::runtime_error("Ошибка создания сокета");
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0)
        {
            throw std::runtime_error("Неверный адрес/адрес не поддерживается");
        }
    }

    ~Client() override
    {
        close(sock);
    }

    bool connectToServer();

    void readFromServer(const std::string &log_file, const std::string &thrname);
};
