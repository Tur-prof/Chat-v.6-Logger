#pragma once

#ifdef _WIN64
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <iostream>
#include <fstream>
#include <filesystem>

#define MESSAGE_LENGTH 1024 // Максимальный размер буфера для данных

class Server
{
public:
    Server();
    ~Server() = default;

	uint16_t PORT;
    std::fstream setting_port;

    // функции работы СЕРВЕРА - SERVER MODE (SM)
#ifdef _WIN64
    void sm_send(SOCKET, char[]);
    void sm_get(SOCKET, char[]);
    void connection();
    void closeSocket();
    SOCKET getSocketClient();
    bool isNumericPort(std::string const&);
   
#elif __linux__
    void sm_send(int, char[]);
    void sm_get(int, char[]);
    void connection();
    void closeSocket();
    int getConnection();
    bool isNumericPort(std::string const&);
#endif

private:

#ifdef _WIN64
    WSADATA wsaData;
    SOCKET server, client;
    struct sockaddr_in serveraddress, clientaddress;
#elif __linux__
    int connect, socket_file_descriptor;
    struct sockaddr_in serveraddress, client;
#endif
    socklen_t length;
};

