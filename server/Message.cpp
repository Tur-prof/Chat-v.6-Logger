#include "Message.h"
#include <iostream>

//Создание сообщения с заданными параметрами

Message::Message(const std::string& from, const std::string& to, const std::string& text, const std::string& time) : _from(from), _to(to), _text(text), _time(time)
{
}

//Возврат логина отправителя сообщения
const std::string Message::getFrom() const
{
    return _from;
}

//Возврат логина получателя сообщения
const std::string Message::getTo() const
{
    return _to;
}

//Возврат текста сообщения
const std::string Message::getText() const
{
    return _text;
}

//Возврат текста сообщения
const std::string Message::getTime() const
{
    return _time;
}

std::istream& operator >> (std::istream& in, Message& msg)
{
    in >> msg._from >> msg._to >> msg._text >> msg._time;
    return in;
}

std::ostream& operator << (std::ostream& out, const Message& msg)
{
    out << msg._from << " " << msg._to << " " << msg._text << " " << msg._time;
    return out;
}
