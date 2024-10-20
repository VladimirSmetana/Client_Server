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
public:
    explicit Socket(int port_number) : port(port_number)
    {
        init_socket();
    }
    int server_fd;
    int port;

    int create_and_bind_socket();
    void listen_connect();
    int accept_socket();
    int init_socket();

    virtual void send_data(int new_socket){};
    ~Socket()
    {
        close(server_fd);
    }
};
