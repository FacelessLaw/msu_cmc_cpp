#pragma once

#include <vector>
#include <string>

const int BUFF_SIZE = 10000;
const int PORT_NUMBER = 8080;
const int BACK_LOG = 10;

const char DELIMETER_SLASH = '/';
const char DELIMETER_QUESTION = '?';
const char DELIMETER_HASH = '#';
const char DELIMETER_EXCL_POINT = '!';

const char *PATH_FROM_MODELS_TO_BIN = "./bin";
const char *PATH_FROM_BIN_TO_MODELES = "./..";

const char *HTML_SERVER_ADDR = "SERVER_ADDR=127.0.0.1";
const char *HTML_CONTENT_TYPE = "CONTENT_TYPE=text/plain";
const char *HTML_SERVER_PROTOCOL = "SERVER_PROTOCOL=HTTP/1.0";
const char *HTML_SCRIPT_NAME = "SCRIPT_NAME=";
const char *HTML_SERVER_PORT = "SERVER_PORT=";
const char *HTML_QUERY_STRING = "QUERY_STRING=";

const std::string HTML_PATH_MAIN = "src_html/main.html";
const std::string HTML_PATH_404 = "src_html/errors/404_error.html";
const std::string HTML_PATH_CGI_ERROR = "src_html/CGI_error.html";

const std::string MSG_SOCKET_CREATION_STR_ERROR = "Error with socket creation!\n";
const std::string MSG_SOCKET_BINDING_STR_ERROR = "Error with socket bind establishment\n";
const std::string MSG_SOCKET_LISTENING_STR_ERROR = "Error with socket listening establishment\n";

const std::string MSG_SERVER_START_SUCCESS = "Server is now running\n";
const std::string MSG_CLIENT_ERROR = "Error with client fd creation\n";
const std::string MSG_SERVER_ERROR = "Error with server recieving\n";

const std::string MSG_FROM_CGI_PROCESS = "Got signal from CGI-process";
const std::string MSG_CGI_PROCESS_FINISHED = "CGI-process finished. Status:";
const std::string MSG_REQUEST_PATH_PREFIX = "Get file path:";
const std::string MSG_ERROR_OPEN_FILE = "Error while opening file:";
