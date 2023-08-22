#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include <shared_mutex>
#include "Message.h"

class Logger
{
	std::fstream _fileLog;
	std::shared_mutex _shared_mutex;

public:

	Logger();
	~Logger();

	void write_log_messages(Message);
	void read_log_messages();

};

