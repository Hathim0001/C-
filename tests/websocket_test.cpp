#include "websocket_client.h"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

TEST(WebSocketClientTest, ConnectAndSend)
{
    WebSocketClient client;
    bool message_received = false;
    std::string received_message;

    // Set up message callback
    client.setMessageCallback([&](const std::string &message)
                              {
        received_message = message;
        message_received = true; });

    // Connect to a local echo server (assumes local_echo_server.py is running)
    ASSERT_TRUE(client.connect("ws://localhost:8765"));

    // Wait for connection to establish
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Send a test message
    std::string test_message = "Hello, WebSocket!";
    ASSERT_TRUE(client.send(test_message));

    // Wait for the echo
    for (int i = 0; i < 10 && !message_received; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    EXPECT_TRUE(message_received);
    EXPECT_EQ(received_message, test_message);

    // Disconnect
    client.disconnect();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}