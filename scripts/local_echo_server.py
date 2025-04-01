#!/usr/bin/env python3

import asyncio
import websockets

async def echo(websocket, path):
    async for message in websocket:
        print(f"Received: {message}")
        await websocket.send(message)
        print(f"Sent: {message}")

start_server = websockets.serve(echo, "localhost", 8765)

print("Local WebSocket echo server running on ws://localhost:8765")
asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()