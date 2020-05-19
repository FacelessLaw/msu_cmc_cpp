#include "server.hpp"
#include "../config.hpp"
#include "../lib/utils.cpp"

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime> 
#include <vector>
#include <string.h>
#include <signal.h>
#include <sys/select.h>
#include <sstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

using namespace std;

Server::Server(string log_file)
{
    this->log_fd = open(log_file.c_str(),O_CREAT | O_WRONLY,0777);
    time_t st_time = time(NULL);
    char* str = asctime(localtime(&st_time));
    write(this->log_fd,str,strlen(str));
    
    this->port = PORT_NUMBER;
    struct sockaddr_in server_address;
    if ((this->server_fd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        cout << MSG_SOCKET_CREATION_STR_ERROR;
        exit(1);
    }
    
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(this->port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(this->server_fd, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        cout << MSG_SOCKET_BINDING_STR_ERROR;
        close(this->server_fd);
        exit(1);
    }
    if(listen(this->server_fd, BACK_LOG) < 0) {
        cout << MSG_SOCKET_LISTENING_STR_ERROR;
        close(this->server_fd);
        exit(1);
    }
}

void Server::close_connection(string message, int &client_fd)
{
    cout << message;
    close(this->server_fd);
    close(client_fd);
    exit(1);
}

void Server::cgi_handler(int &client_fd, bool &is_home_page, char *path)
{
    cout << "NOT IMPLEMENTED";
}

void Server::get_handle(int &client_fd)
{
    bool is_home_page = true;
    int path_end_it = 5;
    while(this->request[path_end_it] != ' ')
        ++path_end_it;

    char path[path_end_it - 3];
    if(path_end_it != 5) {
        is_home_page = false;
        copy(&(this->request[5]), &(this->request[path_end_it]), &path[0]);
        path[path_end_it - 4] = '\0';
    } else {
        path[0] = '/';
        path[1] = '\0';
    }
    
    cout << MSG_REQUEST_PATH_PREFIX << path << endl;
    
    if (is_cgi_format_request(path))
        this->cgi_handler(client_fd, is_home_page, path);
    else {
        int file_fd;
        if (!is_home_page) {
            file_fd = open(path,O_RDONLY);
        }
        if (file_fd < 0) {
            char* res = Response(404, "", true);
            send(client_fd, res, strlen(res), 0);
            free(res);
            resp_file(client_fd, HTML_PATH_404);
        } else {
            char buf[BUFF_SIZE];
            int len = 0;
            char* res = Response(200, "OK", false);
            send(client_fd, res, strlen(res),0);
            free(res);
            
            if (!is_home_page)
                resp_file(client_fd, path);
            else
                resp_file(client_fd, HTML_PATH_MAIN);
        }
    }
}

void Server::run()
{
    cout<< MSG_SERVER_START_SUCCESS;
    while(true) {
        struct sockaddr_in client_address;
        size_t ClAddrLen = sizeof(client_address);
        int client_fd = accept(this->server_fd, (struct sockaddr *) &client_address, (socklen_t *) &ClAddrLen);
        if (client_fd < 0)
            this->close_connection(MSG_CLIENT_ERROR, client_fd);
        
        int req = recv(client_fd, this->request, BUFF_SIZE, 0);
        if(req < 0)
            this->close_connection(MSG_SERVER_ERROR, client_fd);

        
        write(this->log_fd, this->request, strlen(this->request));
        if(!strncmp(this->request, "GET", 3))
            this->get_handle(client_fd);
        else {
            char* res = Response(501, "Not Implemented", true);
            send(client_fd, res, strlen(res), 0);
            free(res);
        }

        shutdown(client_fd, SHUT_RDWR);
        close(client_fd);
    }
}

Server::~Server()
{
    close(this->server_fd);
    close(this->log_fd);
}
