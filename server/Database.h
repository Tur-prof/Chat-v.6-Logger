#pragma once

#include "User.h"
#include "Message.h"
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <filesystem>
#include <cstring>
#ifdef _WIN64
#include <mysql.h>
#else __linux__
#include <mysql/mysql.h>
#endif

class Database
{
    MYSQL_RES* res;
    MYSQL_ROW row;
    MYSQL mysql;

public:

    Database();
    ~Database();

    void connectionDB();
    void createTablesDB();
    void addUserDB(std::string, std::string, std::string);
    void changeStatusByUserDB(std::string, std::string);
    void changeStatusByMessageDB(std::string, std::string);
    void addMessagePublicDB(std::string, std::string, std::string);
    void addMessagePrivateDB(std::string, std::string, std::string, std::string);
    std::string getLoginById(const std::string&);
    std::vector<User> write_vector_users();
    std::vector<Message> write_vector_messagesPrivate();
    std::vector<Message> write_vector_messagesPublic();
    std::map<std::string, bool> write_map_users_online();
    void closeData();

    std::fstream setting;

    std::vector<User> _usersDB;
    std::vector<Message> _messagesDB;
    std::map<std::string, bool> _active_userDB;
};
