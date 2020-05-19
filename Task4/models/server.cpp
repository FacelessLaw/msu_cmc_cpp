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
    chdir(PATH_FROM_MODELS_TO_BIN);
    
    int status;
    string name = to_string(getpid()) + ".txt";
    int file_fd = open(name.c_str(), O_WRONLY|O_CREAT|O_TRUNC, 0644);
    dup2(file_fd, 1);
    
    if(!fork())
    {
        chdir(PATH_FROM_MODELS_TO_BIN);
        string processName = get_fd_name(path);

        int tmp_fd = open (processName.c_str(), O_RDONLY, 0644);
        ifstream is(processName.c_str());
        string s;
        is >> s;
        
        while (s[0] == DELIMETER_HASH || s[0] == DELIMETER_EXCL_POINT || s[0] == ' ')
            s.erase(s.begin());
        
        close(tmp_fd);
        char *argv[] = { (char*)processName.c_str(), NULL };
        
        int offset = search_char(this->request + 5, DELIMETER_QUESTION);
        
        char **envp = new char*[7];
        
        envp[0]=new char[(int)strlen(HTML_SERVER_ADDR) + 1];
        strcpy(envp[0],HTML_SERVER_ADDR);
        envp[1]=new char[(int)strlen(HTML_CONTENT_TYPE) + 1];
        strcpy(envp[1],HTML_CONTENT_TYPE);
        envp[2]=new char[(int)strlen(HTML_SERVER_PROTOCOL) + 1];
        strcpy(envp[2],HTML_SERVER_PROTOCOL);
        envp[3]=new char[(int)strlen(HTML_SCRIPT_NAME) + 1 + strlen(this->request + 5)];
        strcpy(envp[3],HTML_SCRIPT_NAME);
        strcat(envp[3],this->request + 5);
        envp[4]=new char[(int)strlen(HTML_SERVER_PORT) + 5];
        
        char *str_port = itoa(this->port);
        strcpy(envp[4],HTML_SERVER_PORT);
        strcat(envp[4],str_port);
        delete [] str_port;
        envp[5]=new char[(int)strlen(HTML_QUERY_STRING) + 1 + 
                (int)strlen(this->request + 6 + offset)];
        strcpy(envp[5],HTML_QUERY_STRING);
        strcat(envp[5],this->request + 6 + offset);
        envp[6]=NULL;
        
        execve(s.c_str(), argv, envp);
        perror("execve() ");
        exit(1);
    }
    wait(&status);
    close(file_fd);
    if(WIFEXITED(status)) {
        if(WEXITSTATUS(status) == 0) {
            file_fd = open(name.c_str(), O_RDONLY, 0644);
            if (file_fd > 0) {
                char buf[BUFF_SIZE];
                int len = 0;
                char* res = Response(200, "OK", false);
                send(client_fd, res, strlen(res),0);
                free(res);
                while ((len = read(file_fd, &buf, BUFF_SIZE)))
                    send(client_fd,buf,len,0);
                close(file_fd);
            } else {
                string error = MSG_ERROR_OPEN_FILE + " " + name;
                perror(error.c_str());
                char* res = Response(500, "", true);
                send(client_fd, res, strlen(res), 0);
                free(res);
                resp_file(client_fd, HTML_PATH_CGI_ERROR);
            }
        } else {
            string error = MSG_CGI_PROCESS_FINISHED + " " + to_string(WEXITSTATUS(status));
            perror(error.c_str());											
            char* res = Response(500, "", true);
            send(client_fd, res, strlen(res), 0);
            free(res);
            resp_file(client_fd, HTML_PATH_CGI_ERROR);
        }
    }
    else if(WIFSIGNALED(status))
    {
        string error = MSG_FROM_CGI_PROCESS + " " + to_string(WIFSIGNALED(status));
        perror(error.c_str());
        char* res = Response(500, "", true);
        send(client_fd, res, strlen(res), 0);
        free(res);
        resp_file(client_fd, HTML_PATH_CGI_ERROR);
    }

    chdir(PATH_FROM_BIN_TO_MODELES);
}

void Server::get_handle(int &client_fd)
{
    char path[BUFF_SIZE];
    bool is_home_page = true;
    int path_end_it = 5;
    while(this->request[path_end_it] != ' ')
        ++path_end_it;

    if(path_end_it != 5) {
        is_home_page = false;
        copy(&(this->request[5]), &(this->request[path_end_it]), &path[0]);
        path[path_end_it - 4] = '\0';
    } else {
        path[0] = '/';
        path[1] = '\0';
    }
    
    cout << MSG_REQUEST_PATH_PREFIX + " " << path << endl;
    
    if (is_cgi_format_request(path)) {
        this->cgi_handler(client_fd, is_home_page, path);
    }
    else {
        int file_fd;
        if (!is_home_page) {
            file_fd = open(path,O_RDONLY);
            if (file_fd < 0) {
                char* res = Response(404, "", true);
                send(client_fd, res, strlen(res), 0);
                free(res);
                resp_file(client_fd, HTML_PATH_404);
            }
        } else {
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
