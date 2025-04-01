#ifndef CLI_H
#define CLI_H

#include "websocket_client.h"
#include <string>

class CLIHandler
{
public:
    CLIHandler(WebSocketClient &client);
    void run();

private:
    WebSocketClient &client_;
    bool running_;

    void print_help();
    void handle_command(const std::string &command);
};

#endif // CLI_H