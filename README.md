# LuCa

LuCa is a small IRC bot written in C11 with Lua 5.4 as its scripting layer.

M0 provides an IRC parser, TCP client, reconnect loop, PING/PONG, JOIN/PRIVMSG dispatch, Lua events/API, configuration, tests, CI and an Alpine OCI image. Web UI is deliberately out of process; management interoperability is provided through PBMP.

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build

Run with `./build/luca luca.conf`. See `luca.conf.example`.

## Design

The C core owns protocol, networking and lifecycle. Lua scripts receive events and may send IRC messages through a deliberately small API. LuCa remains fully usable without BotWeb or PBMP.

License: MIT.
