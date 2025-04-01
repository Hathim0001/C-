# WebSocket Client Project

This is a C++ WebSocket client application built using the GN build system, as part of an interview task. The client connects to a WebSocket server, sends and receives text messages, and includes a command-line interface (CLI) for user interaction. Bonus features like SSL/TLS support and a local echo server for testing are also implemented.

## Features

- Connects to WebSocket servers (e.g., `wss://echo.websocket.org` or `ws://localhost:8765`)
- Sends and receives text messages
- Interactive CLI with commands: `connect`, `send`, `disconnect`, `help`, `exit`
- SSL/TLS support for secure connections
- Logging for debugging
- Unit tests using Google Test
- Local WebSocket echo server for testing
- Builds with both GCC and Clang

## Prerequisites

- GN and Ninja for building
- OpenSSL for SSL/TLS support
- Python 3 for running the local echo server
- GCC or Clang compiler
- Dependencies (included in `third_party/`):
  - websocketpp
  - Asio (standalone)
  - CLI11
  - Google Test

## Setup

1. Clone the repository and ensure all dependencies are in the `third_party/` directory.
2. Install OpenSSL if not already present (`sudo apt-get install libssl-dev` on Ubuntu).
3. Build Google Test:
   ```bash
   cd third_party/googletest
   mkdir build && cd build
   cmake ..
   make
   ```
