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

const char *PATH_FROM_MODELS_TO_BIN = "./../bin";
const char *PATH_FROM_BIN_TO_MODELES = "./../modeles";

const std::string HTML_PATH_MAIN = "../src_html/main.html";
const std::string HTML_PATH_404 = "../src_html/errors/404_error.html";

const std::string MSG_SOCKET_CREATION_STR_ERROR = "Error with socket creation!\n";
const std::string MSG_SOCKET_BINDING_STR_ERROR = "Error with socket bind establishment\n";
const std::string MSG_SOCKET_LISTENING_STR_ERROR = "Error with socket listening establishment\n";

const std::string MSG_SERVER_START_SUCCESS = "Server is now running\n";
const std::string MSG_CLIENT_ERROR = "Error with client fd creation\n";
const std::string MSG_SERVER_ERROR = "Error with server recieving\n";

const std::string MSG_REQUEST_PATH_PREFIX = "Get file path: ";
