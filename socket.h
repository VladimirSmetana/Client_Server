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

class Socket
{
protected:
    int server_fd;
    int port;

public:
    Socket(int port) : port(port), server_fd(-1) {}

    virtual ~Socket()
    {
        if (server_fd >= 0)
        {
            close(server_fd);
        }
    }

    int init_socket();
    int create_and_bind_socket();
    virtual void send_data(int new_socket) {};
    void listen_connect();

    int accept_socket();
};