#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger
{
public:
    static void init(const std::string &filename);
    static void log(const std::string &message);

private:
    static std::string filename_;
};

#endif // LOGGER_H