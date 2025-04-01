#include "cli.h"
#include "logger.h"
#include <iostream>
#include <sstream>

CLIHandler::CLIHandler(WebSocketClient &client) : client_(client), running_(true)
{
    // Set up the message callback to display received messages
    client_.setMessageCallback([](const std::string &message)
                               {
        std::cout << "\033[1;32mReceived: " << message << "\033[0m\n> ";
        std::cout.flush(); });
}

void CLIHandler::run()
{
    std::cout << "WebSocket Client CLI\nType 'help' for commands.\n> ";
    std::string input;

    while (running_)
    {
        std::getline(std::cin, input);
        if (input.empty())
        {
            std::cout << "> ";
            continue;
        }

        handle_command(input);
        if (running_)
            std::cout << "> ";
    }
}

void CLIHandler::print_help()
{
    std::cout << "Commands:\n"
              << "  connect <uri> - Connect to a WebSocket server (e.g., wss://echo.websocket.org)\n"
              << "  send <message> - Send a message to the server\n"
              << "  disconnect - Disconnect from the server\n"
              << "  help - Show this help message\n"
              << "  exit - Exit the application\n";
}

void CLIHandler::handle_command(const std::string &command)
{
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "help")
    {
        print_help();
    }
    else if (cmd == "connect")
    {
        std::string uri;
        iss >> uri;
        if (uri.empty())
        {
            std::cout << "Usage: connect <uri>\n";
            return;
        }
        if (client_.connect(uri))
        {
            std::cout << "Connecting to " << uri << "...\n";
        }
        else
        {
            std::cout << "Failed to initiate connection.\n";
        }
    }
    else if (cmd == "send")
    {
        std::string message;
        std::getline(iss, message);
        if (message.empty())
        {
            std::cout << "Usage: send <message>\n";
            return;
        }
        // Remove leading space
        message = message.substr(1);
        if (!client_.send(message))
        {
            std::cout << "Failed to send message.\n";
        }
    }
    else if (cmd == "disconnect")
    {
        client_.disconnect();
    }
    else if (cmd == "exit")
    {
        client_.disconnect();
        running_ = false;
    }
    else
    {
        std::cout << "Unknown command. Type 'help' for commands.\n";
    }
}