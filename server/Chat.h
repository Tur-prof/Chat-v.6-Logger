#pragma once
#include <vector>
#include <map>
#include <memory>
#include <ctime>

#include "Message.h"
#include "User.h"
#include "Server.h"
#include "Database.h"
#include "Logger.h"

class Chat
{
    Server _server;
    Logger _logger;

public:
    Chat();

    void start();
    bool chatOn() const;
    void loginOn();
    bool checkLogin() const;
    void showLoginMenu();
    void showUserMenu();

    bool login();
    void registrUser();
    void showMenuChatPrivate();
    void showMenuChatPublic();
    void showUsersList();
    void showChatPublic();
    void addMessagePublic();
    void showChatPrivate();
    void addMessagePrivate();
    std::string getDateTime();

#ifdef _WIN64
    void serverOn();
    void checkLoginSM();
    void showUserMenuSM();
    void registrUserSM();
    void showMenuChatPublicSM();
    void showMenuChatPrivateSM();
    void showUsersListSM();
    void showChatPublicSM();
    void addMessagePublicSM();
    void showChatPrivateSM();
    void addMessagePrivateSM();
#elif __linux__
    void serverOn();
    void checkLoginSM();
    void showUserMenuSM();
    void registrUserSM();
    void showMenuChatPublicSM();
    void showMenuChatPrivateSM();
    void showUsersListSM();
    void showChatPublicSM();
    void addMessagePublicSM();
    void showChatPrivateSM();
    void addMessagePrivateSM();
#endif

    ~Chat() = default;

private:

    bool _chatOn = false;
    bool _checkLogin = false;
    bool _clientLogin = false;
    bool server_online = false;
    bool checkLog = false;
    std::string _admin_online;
    std::string _user_online;

    std::vector<std::string> _logins;
    std::vector<User> _users;
    std::vector<Message> _messages;
    std::map<std::string, bool> _active_user;

    std::shared_ptr<Database> _database;


};
