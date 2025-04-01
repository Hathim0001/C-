#include "websocket_client.h"
#include "logger.h"
#include <iostream>

WebSocketClient::WebSocketClient() : is_connected_(false)
{
    // Initialize the client
    client_.clear_access_channels(websocketpp::log::alevel::all);
    client_.clear_error_channels(websocketpp::log::elevel::all);
    client_.init_asio();

    // Set up handlers
    client_.set_open_handler([this](auto hdl)
                             { on_open(hdl); });
    client_.set_message_handler([this](auto hdl, auto msg)
                                { on_message(hdl, msg); });
    client_.set_close_handler([this](auto hdl)
                              { on_close(hdl); });
    client_.set_fail_handler([this](auto hdl)
                             { on_fail(hdl); });

    // Set TLS initialization handler
    client_.set_tls_init_handler([](websocketpp::connection_hdl)
                                 { return std::make_shared<asio::ssl::context>(asio::ssl::context::tlsv12); });
}

WebSocketClient::~WebSocketClient()
{
    disconnect();
    if (io_thread_.joinable())
    {
        io_thread_.join();
    }
}

bool WebSocketClient::connect(const std::string &uri)
{
    if (is_connected_)
    {
        Logger::log("Already connected to a server.");
        return false;
    }

    try
    {
        websocketpp::lib::error_code ec;
        Client::connection_ptr con = client_.get_connection(uri, ec);
        if (ec)
        {
            Logger::log("Connection error: " + ec.message());
            return false;
        }

        handle_ = con->get_handle();
        client_.connect(con);

        // Start the io_context in a separate thread
        io_thread_ = std::thread([this]()
                                 { client_.run(); });
        return true;
    }
    catch (const std::exception &e)
    {
        Logger::log("Exception during connection: " + std::string(e.what()));
        return false;
    }
}

bool WebSocketClient::send(const std::string &message)
{
    if (!is_connected_)
    {
        Logger::log("Not connected to any server.");
        return false;
    }

    try
    {
        client_.send(handle_, message, websocketpp::frame::opcode::text);
        Logger::log("Sent message: " + message);
        return true;
    }
    catch (const std::exception &e)
    {
        Logger::log("Exception during send: " + std::string(e.what()));
        return false;
    }
}

void WebSocketClient::disconnect()
{
    if (!is_connected_)
        return;

    try
    {
        client_.close(handle_, websocketpp::close::status::normal, "Client disconnect");
        client_.stop();
        is_connected_ = false;
        Logger::log("Disconnected from server.");
    }
    catch (const std::exception &e)
    {
        Logger::log("Exception during disconnect: " + std::string(e.what()));
    }
}

void WebSocketClient::setMessageCallback(std::function<void(const std::string &)> callback)
{
    message_callback_ = callback;
}

void WebSocketClient::on_open(websocketpp::connection_hdl hdl)
{
    is_connected_ = true;
    Logger::log("Connected to server.");
}

void WebSocketClient::on_message(websocketpp::connection_hdl hdl, MessagePtr msg)
{
    std::string message = msg->get_payload();
    Logger::log("Received message: " + message);
    if (message_callback_)
    {
        message_callback_(message);
    }
}

void WebSocketClient::on_close(websocketpp::connection_hdl hdl)
{
    is_connected_ = false;
    Logger::log("Connection closed.");
}

void WebSocketClient::on_fail(websocketpp::connection_hdl hdl)
{
    is_connected_ = false;
    Logger::log("Connection failed: " + client_.get_con_from_hdl(hdl)->get_ec().message());
}