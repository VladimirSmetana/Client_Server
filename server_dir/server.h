
#include "../socket.h"
class Server : public Socket
{
public:
    explicit Server(int port_number) : Socket(port_number) {};
    void send_data(int new_socket) override;

private:
    int getcurtime(char *buffer, size_t bufferSize);
};
