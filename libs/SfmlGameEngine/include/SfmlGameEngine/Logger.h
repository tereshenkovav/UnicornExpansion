#pragma once

#include <string>
#include <fstream>

namespace sfge {

class Logger
{
public:
    virtual void WriteLog(const std::string & str) ;
    virtual void Flush() ;
};

// Примитивная реализация, на её основе можно сделать кэширующую версию, которая пишет данные только по flush()
class LoggerFile: public Logger
{
private:
    std::ofstream log;
public:
    LoggerFile(const std::string& filename);
    virtual ~LoggerFile();
    virtual void WriteLog(const std::string& str) override;
    virtual void Flush() override;
};

};
