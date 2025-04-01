#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <functional>
#include <string>

typedef websocketpp::client<websocketpp::config::asio_tls_client> Client;
typedef websocketpp::config::asio_tls_client::message_type::ptr MessagePtr;

class WebSocketClient
{
public:
    WebSocketClient();
    ~WebSocketClient();

    // Connect to a WebSocket server
    bool connect(const std::string &uri);

    // Send a text message
    bool send(const std::string &message);

    // Disconnect from the server
    void disconnect();

    // Set callback for received messages
    void setMessageCallback(std::function<void(const std::string &)> callback);

private:
    Client client_;
    websocketpp::connection_hdl handle_;
    std::function<void(const std::string &)> message_callback_;
    bool is_connected_;

    // Thread for running the io_context
    std::thread io_thread_;

    // Handlers for WebSocket events
    void on_open(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, MessagePtr msg);
    void on_close(websocketpp::connection_hdl hdl);
    void on_fail(websocketpp::connection_hdl hdl);
};

#endif // WEBSOCKET_CLIENT_H