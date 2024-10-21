#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <thread>
#include "server.h"
#include <vector>

void Server::send_data(int new_socket)
{
    char buffer[100];

    while (true)
    {
        getcurtime(buffer, sizeof(buffer));
        send(new_socket, buffer, strlen(buffer), 0);
        sleep(1);
    }
}

int Server::getcurtime(char *buffer, size_t bufferSize)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    time_t rawtime = tv.tv_sec;
    struct tm *timeinfo = localtime(&rawtime);
    strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", timeinfo);

    snprintf(buffer + strlen(buffer), bufferSize - strlen(buffer), ".%03ld", tv.tv_usec / 1000);

    puts(buffer);
    return 0;
}

void handle_client(Server &server, int new_socket)
{
    server.send_data(new_socket);
    close(new_socket);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return EXIT_FAILURE;
    }

    Server S(atoi(argv[1]));
    S.init_socket();
    std::vector<std::thread> threads;

    while (true)
    {
        int new_socket = S.accept_socket();
        if (new_socket >= 0)
        {
            threads.emplace_back(handle_client, std::ref(S), new_socket);
        }
    }

    for (auto &thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    return 0;
}