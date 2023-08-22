#include "Logger.h"
#include <iostream>

namespace fs = std::filesystem;

Logger::Logger()
{
    _fileLog.open("log.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    fs::permissions("log.txt", fs::perms::owner_all);
    if (!_fileLog.is_open())
    {
        std::cout << "File <log.txt> openning error!" << std::endl;
    }
}

Logger::~Logger()
{
    _fileLog.close();
}

//Функция считывания файла с данными пользователей
void Logger::write_log_messages(Message msg)
{
    _shared_mutex.lock();
    _fileLog << msg << std::endl;
    _shared_mutex.unlock();
}

//Функция считывания файла сообщений
void Logger::read_log_messages()
{
    std::string from;
    std::string to;
    std::string text;
    std::string time;
    _shared_mutex.lock_shared();
    while (getline(_fileLog, from, ' ') && getline(_fileLog, to, ' ') && getline(_fileLog, text, ' ') && getline(_fileLog, time))
        {
        std::cout << time << " || From " << from << " To " << to << " : " << text << std::endl;
        }
    _shared_mutex.unlock_shared();
    
}