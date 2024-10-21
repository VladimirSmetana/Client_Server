

#include "client.h"

bool Client ::connectToServer()
{
    return connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) >= 0;
}

void Client ::readFromServer(const std::string &log_file, const std::string &thrname)
{
    std::ofstream file(log_file, std::ios::app); // Открываем файл в режиме добавления
    if (!file.is_open())
    {
        throw std::runtime_error("Не удалось открыть файл для записи");
    }

    while (true)
    {
        ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);

        if (bytes_read < 0)
        {
            std::cerr << "Ошибка чтения из сокета" << std::endl;
            break;
        }
        else if (bytes_read == 0)
        {
            std::cout << "Соединение закрыто сервером" << std::endl;
            break;
        }

        buffer[bytes_read] = '0'; // Завершаем строку нулевым символом

        {
            std::lock_guard<std::mutex> lock(file_mutex);
            std::cout << " [" << buffer << "] " << thrname << std::endl;
            file << " [" << buffer << "] " << thrname << std::endl;
        }
    }

    file.close(); // Закрываем файл
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Использование: " << argv[0] << " <имя> <порт>" << std::endl;
        return -1;
    }

    try
    {
        Client client("127.0.0.1", atoi(argv[2]));

        if (!client.connectToServer())
        {
            std::cerr << "Ошибка подключения к серверу" << std::endl;
            return -1;
        }

        client.readFromServer("log.txt", argv[1]);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
