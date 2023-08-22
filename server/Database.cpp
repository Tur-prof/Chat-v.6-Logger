#include"Database.h"

namespace fs = std::filesystem;

Database::Database()
{
    connectionDB();
}

Database::~Database()
{
    closeData();
}

    // подключаемся к БД
void Database::connectionDB()
{
    mysql_init(&mysql);
    if (&mysql == nullptr)
    {
        std::cout << "Error: can't create MySQL-descriptor" << std::endl;
        exit(1);
    }
    std::string hostDB;
    std::string loginDB;
    std::string passwordDB;
    std::string nameDB;
    setting.open("setting_database.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    fs::permissions("setting_database.txt", fs::perms::owner_all);
    if (!setting.is_open())
    {
        std::cout << "File openning error!" << std::endl;
        std::cout << "Enter the host (for example localhost): " << std::endl;
        std::cin >> hostDB;
        std::cout << "Enter the login (for example root): " << std::endl;
        std::cin >> loginDB;
        std::cout << "Enter the password (for example tur-prof): " << std::endl;
        std::cin >> hostDB;
        std::cout << "Enter the name database (for example chatdb): " << std::endl;
        std::cin >> nameDB;
    }
    else
    {
        fs::permissions("setting_database.txt", fs::perms::owner_all);
        while (getline(setting, hostDB, ' ') && getline(setting, loginDB, ' ') && getline(setting, passwordDB, ' ') && getline(setting, nameDB))
        {
        }
    }
    setting.close();
    MYSQL* connect = mysql_real_connect(&mysql, hostDB.c_str(), loginDB.c_str(), passwordDB.c_str(), nameDB.c_str(), 0, NULL, 0);
    if (connect == nullptr)
    {
        std::cout << "Error: can't connect to database " << mysql_error(&mysql) << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "Success connected to DB" << std::endl;
    }
    createTablesDB();
    mysql_set_character_set(&mysql, "cp1251");
    std::cout << "connection characterset: " << mysql_character_set_name(&mysql) << std::endl;
}

void Database::createTablesDB()
{
    // создаем таблицу пользователей
    const std::string str = "create table if not exists users"
        "(id int auto_increment primary key not null, "
        "login varchar(20) unique not null, "
        "password varchar(20) not null, "
        "status varchar(10) not null)";
    mysql_query(&mysql, str.c_str());
    if (mysql_errno(&mysql))
    {
        std::cout << "Table users is not created or not finded!" << mysql_error(&mysql) << std::endl;
    }
    else
    {
        std::cout << "Table users is created or finded!" << std::endl;
    }
    // создаем таблицу приватных сообщений
    const std::string strr = "create table if not exists messages_private"
        "(id int auto_increment primary key not null, "
        "from_id integer, foreign key (from_id) references users(id) on delete cascade on update cascade, "
        "to_id integer, foreign key(to_id) references users(id) on delete cascade on update cascade, "
        "text varchar(255), date_time varchar(100))";
    mysql_query(&mysql, strr.c_str());
    if (mysql_errno(&mysql))
    {
        std::cout << "Table messages_private is not created or not finded!" << mysql_error(&mysql) << std::endl;
    }
    else
    {
        std::cout << "Table messages_private is created or finded!" << std::endl;
    }
    const std::string strrr = "create table if not exists messages_public"
        "(id int auto_increment primary key not null, "
        "from_id integer, foreign key (from_id) references users(id) on delete cascade on update cascade, "
        "text varchar(255), date_time varchar(100))";
    mysql_query(&mysql, strrr.c_str());
    if (mysql_errno(&mysql))
    {
        std::cout << "Table messages_public is not created or not finded!" << mysql_error(&mysql) << std::endl;
    }
    else
    {
        std::cout << "Table messages_public is created or finded!" << std::endl;
    }
}

    // добавляем пользователя в БД
void Database::addUserDB(std::string login, std::string password, std::string status)
{
    const std::string str = "insert into users (id, login, password, status) values (default, '" + login + "', '" + password + "','" + status + "')";
     
    mysql_query(&mysql, str.c_str());
    if (mysql_errno(&mysql))
    {
        std::cout << "\033[1;31m" << "User is not add to BD!" << mysql_error(&mysql) << std::endl;
    }
    else
    {
        std::cout << "\033[1;31m" << "REGISTRATION COMPLETED SUCCESSFULLY!" << std::endl;
    }
}

    // добавляем приватные сообщения в БД
void Database::addMessagePrivateDB(std::string from, std::string to, std::string text, std::string time)
{
    const std::string str = "insert into messages_private (id, from_id, to_id, text, date_time) values (default, \
	 (select id from users where login = '" + from + "'), (select id from users where \
     login ='" + to + "'), '" + text + "', '" + time + "')";
    mysql_query(&mysql, str.c_str());

    if (mysql_errno(&mysql))
    {
        std::cout << "Message is not add to BD!" << mysql_error(&mysql) << std::endl;
    }
}

void Database::addMessagePublicDB(std::string from, std::string text, std::string time)
{
    const std::string str = "insert into messages_public (id, from_id, text, date_time) values (default, \
	 (select id from users where login = '" + from + "'), '" + text + "', '" + time + "')";
    mysql_query(&mysql, str.c_str());

    if (mysql_errno(&mysql))
    {
        std::cout << "Message is not add to BD!" << mysql_error(&mysql) << std::endl;
    }
}

    // получение логина по ID в БД
std::string Database::getLoginById(const std::string& _id)
{
   std::string str = "SELECT login FROM users where id = '" + _id + "'";
   mysql_query(&mysql, str.c_str());
   MYSQL_RES* res;
   MYSQL_ROW row;
   if (res = mysql_store_result(&mysql))
   {
       row = mysql_fetch_row(res);
       return row[0];
   }
}

    // смена статуса пользователя в БД
void Database::changeStatusByUserDB(std::string login, std::string status)
{
    const std::string str = "update users set status = '" + status + "' where login = '" + login + "'";
    mysql_query(&mysql, str.c_str());

    if (mysql_errno(&mysql))
    {
        std::cout << "Status not change to BD!" << mysql_error(&mysql) << std::endl;
    }
}

    // чтение и запись данных пользователей в вектор
std::vector<User> Database::write_vector_users()
{
    const std::string str = "SELECT * FROM users";
    mysql_query(&mysql, str.c_str());
    std::string login;
    std::string password;
    std::string status;
    if (res = mysql_store_result(&mysql))
    {
        while (row = mysql_fetch_row(res))
        {
            for (auto i = 1; i < mysql_num_fields(res); ++i)
            {
                login = row[i];
                password = row[i + 1];
                User _user = User(login, password);
                _usersDB.emplace_back(_user);
                break;
            }
        }
        std::cout << "Users Database read successfully!" << std::endl;
    }
    return _usersDB;
}

    // чтение и запись приватных сообщений пользователей в вектор
std::vector<Message> Database::write_vector_messagesPrivate()
{
    const std::string str = "SELECT * FROM messages_private";
    mysql_query(&mysql, str.c_str());
    std::string from;
    std::string to;
    std::string text;
    std::string time;
    if (res = mysql_store_result(&mysql))
    {
        while (row = mysql_fetch_row(res))
        {
            for (int i = 1; i < mysql_num_fields(res); ++i)
            {
                from = getLoginById(row[i]);
                to = getLoginById(row[i + 1]);
                text = row[i + 2];
                time = row[i + 3];
                Message _message = Message(from, to, text, time);
                _messagesDB.emplace_back(_message);
                break;
            }
        }
        std::cout << "Messages Private Database read successfully!" << std::endl;
    }
    return _messagesDB;
}

// чтение и запись публичных сообщений пользователей в вектор
std::vector<Message> Database::write_vector_messagesPublic()
{
    const std::string str = "SELECT * FROM messages_public";
    mysql_query(&mysql, str.c_str());
    std::string from;
    std::string to;
    std::string text;
    std::string time;
    if (res = mysql_store_result(&mysql))
    {
        while (row = mysql_fetch_row(res))
        {
            for (int i = 1; i < mysql_num_fields(res); ++i)
            {
                from = getLoginById(row[i]);
                to = "All";
                text = row[i + 1];
                time = row[i + 2];
                Message _message = Message(from, to, text, time);
                _messagesDB.emplace_back(_message);
                break;
            }
        }
        std::cout << "Messages Public Database read successfully!" << std::endl;
    }
    return _messagesDB;
}

    // чтение и запись статуса пользователей
std::map<std::string, bool> Database::write_map_users_online()
{
    const std::string str = "SELECT * FROM users";
    mysql_query(&mysql, str.c_str());
    std::string login;
    std::string status;
    if (res = mysql_store_result(&mysql))
    {
        while (row = mysql_fetch_row(res))
        {
            for (auto i = 1; i < mysql_num_fields(res); ++i)
            {
                login = row[i];
                status = row[i + 2];
                if (status == "0")
                {
                    _active_userDB[login] = false;
                }
                else _active_userDB[login] = true;
                break;
            }
        }
    }
    return _active_userDB;
}

void Database::closeData()
{
    mysql_close(&mysql);
}
