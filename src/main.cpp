#include "cli.h"
#include "logger.h"
#include <iostream>
#include <thread>

int main()
{
    Logger::init("websocket_client.log"); // Initialize logger
    Logger::log("Starting WebSocket client application...");

    // Create WebSocket client and CLI handler
    WebSocketClient client;
    CLIHandler cli(client);

    // Run the CLI in the main thread
    cli.run();

    Logger::log("Application terminated.");
    return 0;
}