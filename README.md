# LuCa

LuCa is a small IRC bot written in C11 with Lua 5.4 as its scripting layer.

M0 provides the IRC core, Lua event/API layer, tests, CI and Alpine OCI. M0.1 adds TLS with certificate/hostname verification, IRCv3 CAP negotiation and optional SASL PLAIN authentication. TLS is enabled by default; plaintext IRC requires `tls=false` explicitly.

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build

Run with `./build/luca luca.conf`. See `luca.conf.example`.

## Design

The C core owns protocol, networking, security-sensitive transport and lifecycle. Lua scripts receive events and may send IRC messages through a deliberately small API. LuCa remains fully usable without BotWeb or PBMP.

The web UI is deliberately out of process. Management interoperability is defined by the separate PBMP project.

License: MIT.
