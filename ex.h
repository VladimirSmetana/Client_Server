#include <cstdint>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

class Socket {
public:
    Socket() : newSD(-1), serverSd(-1), send_ofs(0) {}
    ~Socket() { uninit_socket(); }

    void init_socket(int p_number);
    void uninit_socket();
    int send_data(int p_number, auto  reader, size_t send_size);

private:
    int newSD;
    int serverSd;
    int send_ofs;
};

void Socket::init_socket(int p_number) {
    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(p_number);

    serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSd < 0) {
        std::cerr << "Error establishing the server socket" << std::endl;
        newSD = -1; // Устанавливаем флаг ошибки
        return;
    }

    const int enable = 1;
    if (setsockopt(serverSd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        std::cerr << "Error setting socket address reuse" << std::endl;
        close(serverSd);
        newSD = -1; // Устанавливаем флаг ошибки
        return;
    }

    if (bind(serverSd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        std::cerr << "Error binding socket to local address" << std::endl;
        close(serverSd);
        newSD = -1; // Устанавливаем флаг ошибки
        return;
    }

    listen(serverSd, 1);
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);

    newSD = accept(serverSd, (sockaddr*)&newSockAddr, &newSockAddrSize);
    if (newSD < 0) {
        std::cerr << "Error accepting request from client!" << std::endl;
        close(serverSd);
        newSD = -1; // Устанавливаем флаг ошибки
        return;
    }

    std::cout << "Connected with client!" << std::endl;

    int flags = fcntl(newSD, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(newSD, F_SETFL, flags);
}

void Socket::uninit_socket() {
    if (newSD >= 0) close(newSD);
    if (serverSd >= 0) close(serverSd);
}

int Socket::send_data(int p_number, auto reader, size_t send_size) {
    if (newSD < 0) {
        init_socket(p_number);
        if (newSD < 0) return -1; // Ошибка инициализации
    }

    if (send_size > 0) {
        ssize_t ret = send(newSD, reader + send_ofs, send_size - send_ofs, MSG_NOSIGNAL);
        if (ret > 0) {
            send_ofs += ret;
            if (send_size <= send_ofs) {
                send_size = 0;
                send_ofs = 0;
            }
            return 1; // Успешная отправка
        } else if (errno != EAGAIN) {
            if (errno == EPIPE) {
                uninit_socket();
                init_socket(p_number);
            }
            std::cerr << "Send failed errno " << errno << std::endl;
            return 0; // Ошибка отправки
        }
    }
    return 0; // Ничего не отправлено
}
