#include "logger.h"
#include <fstream>
#include <ctime>

std::string Logger::filename_;

void Logger::init(const std::string &filename)
{
    filename_ = filename;
    std::ofstream file(filename_, std::ios::app);
    if (file.is_open())
    {
        auto now = std::time(nullptr);
        file << "=== Log started at " << std::ctime(&now) << "===\n";
        file.close();
    }
}

void Logger::log(const std::string &message)
{
    std::ofstream file(filename_, std::ios::app);
    if (file.is_open())
    {
        auto now = std::time(nullptr);
        file << "[" << std::ctime(&now) << "] " << message << "\n";
        file.close();
    }
}