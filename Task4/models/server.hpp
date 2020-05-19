#pragma once

#include <string>
#include "../config.hpp"

using namespace std;

class Server
{
private:
    int server_fd; // fd для сервера
    int port; // номер порта для сервера
    char request[BUFF_SIZE]; // содержание запроса
    int log_fd; // логи
public:
    Server(string log_file); // конструктор по log-файлу
    void run(); // запуск сервера
    void close_connection(const string message, int &client_fd); // экстренное выключение
    void get_handle(int &client_fd); // GET-ручка
    void cgi_handler(int &client_fd, bool &is_home_page, char *path); // Обработчик cgi-запросов
    ~Server();
};


