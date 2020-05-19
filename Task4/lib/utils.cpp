#pragma once

#include "../config.hpp"

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

char* Response(int code, string msg, bool with_body){ // Формирование ответа на запрос
    string str = "HTTP/1.1 ";
    str += to_string(code);
    str += " ";
    str += msg;
    str += "\r\nStatus: ";
    str += to_string(code);
    str += " ";
    str += msg;
    
    if(with_body) {
        str += "\r\nContent-type: text/html; charset=utf-8\r\n\r\n<html><body><p>";
        str += msg;
        str += "</p></body></html>";
    } else {
        str += "\r\nContent-type: text/html; charset=utf-8\r\n\r\n";
    }
    
    int n = str.length();
    char* res = (char*)malloc((n + 1)*sizeof(char));
    strcpy(res, str.c_str());
    
    return res;
}

void resp_file(int client_fd, string error_file) // Формирование файла для ответа
{
	int len=0;
	char buf[BUFF_SIZE];
	int file_fd = open(error_file.c_str(), O_RDONLY);
	while ((len = read(file_fd, &buf, BUFF_SIZE))) {
		if (send(client_fd, buf, len, 0)<0) {
			close(file_fd);
			perror(error_file.c_str());
			shutdown(client_fd, SHUT_RDWR);
			close(client_fd);
			exit(1);
		}
	}
	close(file_fd);
}

char* get_params(char* str)
{
    char* params = new char[BUFF_SIZE];
    int i = 5, j = 0;
    while(str[i] != 0) {
        if(str[i] != DELIMETER_SLASH) 
            params[j++] = str[i];
        else 
            j = 0;
        i++;
    }
    params[j] = 0;
    return params;
}

bool is_cgi_format_request(char* str) // Проверка запроса на формат cgi-bin
{
    char* params = get_params(str);
    bool is_found_key_word = false;
    for(size_t i = 0; !is_found_key_word && i < strlen(params); i++) {
        if(params[i] == DELIMETER_QUESTION)
            is_found_key_word = true;
    }

    delete [] params;
    return is_found_key_word;
}

char* get_fd_name(char* str) // Находит имя файла в URI запроса
{
    char* first = new char[BUFF_SIZE];
    strcpy(first, str);
    int i = 0;
    
    while(first[i++] != DELIMETER_QUESTION);
    
    first[i-1] = 0;
    return first;
}

char * itoa(int i) // Рукописный каст int в string
{
	char * str =new char[11];
	sprintf(str,"%d",i);
	return str;
}

int search_char(char * str, char c) // Поиск символа в строке
{
	int j = 0;
	while((str[j] != '\0') && (str[j] != c))
        j++;

	if(str[j] == '0')
        return 0;
	
    return j;
}
