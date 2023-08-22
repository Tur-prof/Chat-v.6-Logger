#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Server.h"
#include <cstring>
#include <string>

namespace fs = std::filesystem;

Server::Server()
{
    std::string _port;
    setting_port.open("setting_server.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    fs::permissions("setting_server.txt", fs::perms::owner_all);
    if (!setting_port.is_open())
    {
        std::cout << "File openning error!" << std::endl;
        std::cout << "Enter the port (for example 7777): " << std::endl;
        std::cin >> _port;
        while (true)
        {
            if (isNumericPort(_port))
            {
                PORT = static_cast<uint16_t>(std::stoi(_port));
                break;
            }
            else
            {
                std::cout << "Enter the port (for example 7777): " << std::endl;
                std::cin >> _port;
            }
        }
    }
    else
    {
        fs::permissions("setting_server.txt", fs::perms::owner_all);
        while (getline(setting_port, _port, '\n'))
        {
        }
        while (true)
        {
            if (isNumericPort(_port))
            {
                PORT = static_cast<uint16_t>(std::stoi(_port));
                break;
            }
            else
            {
                std::cout << "Enter the port (for example 7777): " << std::endl;
                std::cin >> _port;
            }
        }
    }
    setting_port.close();
}

bool Server::isNumericPort(std::string const& str)
{
    int it = 0;
    for (auto i = 0; i < str.size(); i++)
    {
        if (std::isdigit(str[i]))
        {
            it++;
        }
    }
    return !str.empty() && it == str.size();
}


#ifdef _WIN64
void Server::sm_send(SOCKET client, char* i_message)
{
    size_t bytes = send(client, i_message, MESSAGE_LENGTH, 0);
    // Если передали >= 0  байт, значит пересылка прошла успешно
    if (bytes >= 0)
    {
        //std::cout << "Data successfully sent to the client.!" << std::endl;
    }
    //WSACleanup();
}

void Server::sm_get(SOCKET client, char* i_message)
{
    recv(client, i_message, MESSAGE_LENGTH, 0);
    if (strncmp("QQQ", i_message, 3) == 0) {
        std::cout << "Client Exited." << std::endl;
        std::cout << "Server is Exiting..!" << std::endl;
        return;
    }
    //std::cout << "Data received from client: " << i_message << std::endl;
    //WSACleanup();
}

void Server::connection()
{
    int check_status, bind_status, connection_status;


    check_status = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (check_status != 0)
    {
        std::cout << "WSAStartup failed with error: " << WSAGetLastError() << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "WinSock successfully initialized" << std::endl;
    }

    // Создадаем сокет
    server = socket(AF_INET, SOCK_STREAM, 0);
    // если не удалось 
    if (server == -1) {
        std::cout << "Socket creation failed!" << std::endl;
        closesocket(server);
        WSACleanup();
        exit(1);
    }
    // содержит IP-адрес хоста
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // Зададим номер порта для связи
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;

    // Привяжем сокет
    bind_status = bind(server, (sockaddr*)&serveraddress, sizeof(serveraddress));
    // если не удалось
    if (bind_status != 0) {
        std::cout << "Socket binding failed!" << std::endl;
        closesocket(server);
        WSACleanup();
        exit(1);
    }
    else
    {
        std::cout << "Server socket successfully initialized" << std::endl;
    }

    // сокет переводится в режим ожидания запросов со стороны клиентов
    connection_status = listen(server, SOMAXCONN);
    // если не удалось
    if (connection_status != 0) {
        std::cout << "Socket is unable to listen for new connections!" << std::endl;
        closesocket(server);
        WSACleanup();
        exit(1);
    }
    else {
        std::cout << "Server is listening for new connection..." << std::endl;
    }

    ZeroMemory(&clientaddress, sizeof(clientaddress));
    int client_size = sizeof(clientaddress);
    client = accept(server, (sockaddr*)&clientaddress, &client_size);
    if (client == INVALID_SOCKET)
    {
        std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
        closesocket(server);
        closesocket(client);
        WSACleanup();
        exit(1);
    }
    else
    {
        std::cout << "Server: got connection from " << inet_ntoa(clientaddress.sin_addr) << " port " << ntohs(clientaddress.sin_port) << std::endl;
    }
}

void Server:: closeSocket()
{
    closesocket(server);
    closesocket(client);
    WSACleanup();
}

SOCKET Server::getSocketClient()
{
    return client;
}

// Далее идут функции работы СЕРВЕРА - SERVER MODE (SM) для __linux__

#else __linux__
void Server::sm_send(int connect, char* i_message)
{
    ssize_t bytes = write(connect, i_message, MESSAGE_LENGTH);
    // Если передали >= 0  байт, значит пересылка прошла успешно
    if (bytes >= 0)
    {
        //std::cout << "Data successfully sent to the client.!" << std::endl;
    }

}

void Server::sm_get(int connect, char* i_message)
{
    read(connect, i_message, MESSAGE_LENGTH);
    if (strncmp("QQQ", i_message, 3) == 0) {
        std::cout << "Client Exited." << std::endl;
        std::cout << "Server is Exiting..!" << std::endl;
        return;
    }
    //std::cout << "Data received from client: " << i_message << std::endl;
}

void Server::connection()
{
    int bind_status, connection_status;
    char msg[MESSAGE_LENGTH];
    std::string buff_mess;
    bzero(msg, MESSAGE_LENGTH);
    // Создадаем сокет
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    // если не удалось 
    if (socket_file_descriptor == -1) {
        std::cout << "Socket creation failed!" << std::endl;
        exit(1);
    }
    // содержит IP-адрес хоста
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // Зададим номер порта для связи
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;

    // Привяжем сокет
    bind_status = bind(socket_file_descriptor, (struct sockaddr*)&serveraddress,
        sizeof(serveraddress));
    // если не удалось
    if (bind_status == -1) {
        std::cout << "Socket binding failed!" << std::endl;
        exit(1);
    }


    // сокет переводится в режим ожидания запросов со стороны клиентов
    connection_status = listen(socket_file_descriptor, 5);
    // если не удалось
    if (connection_status == -1) {
        std::cout << "Socket is unable to listen for new connections!" << std::endl;
        exit(1);
    }
    else {
        std::cout << "Server is listening for new connection..." << std::endl;
    }

    length = sizeof(client);
    // создаём для общения с клиентом новый сокет и возвращаем его дескриптор
    connect = accept(socket_file_descriptor, (struct sockaddr*)&client, &length);
    // если не удалось
    if (connect == -1) {
        std::cout << "Server is unable to accept the data from client!" << std::endl;
        exit(1);
    }

    std::cout << "Server: got connection from " << inet_ntoa(client.sin_addr) << " port " << ntohs(client.sin_port) << std::endl;

}

void Server::closeSocket()
{
    close(socket_file_descriptor);
}

int Server::getConnection()
{
    return connect;
}
#endif

