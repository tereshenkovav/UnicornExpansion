#include "SfmlGameEngine/Logger.h"

namespace sfge {

// Ничего не делает
void Logger::WriteLog(const std::string & str) {
}

// Ничего не делает
void Logger::Flush() {
}

LoggerFile::LoggerFile(const std::string& filename)
{
	log.open(filename);	
}

LoggerFile::~LoggerFile()
{
	Flush();
	log.close();
}

void LoggerFile::WriteLog(const std::string& str) {
	log << str << std::endl;
}

// Ничего не делает
void LoggerFile::Flush() {
	log.flush();
}

}
