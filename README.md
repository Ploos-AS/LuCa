# LuCa

LuCa is a small IRC bot written in C11 with Lua 5.4 as its scripting layer.

M0 provides the IRC core, Lua event/API layer, tests, CI and Alpine OCI. M0.1 adds TLS with certificate/hostname verification, IRCv3 CAP negotiation and optional SASL PLAIN authentication. TLS is enabled by default; plaintext IRC requires `tls=false` explicitly.

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build

Run with `./build/luca luca.conf`. See `luca.conf.example`.

## Design

The C core owns protocol, networking, security-sensitive transport and lifecycle. Lua scripts receive events and may send IRC messages through a deliberately small API.

### Standalone-first rule

LuCa MUST remain a fully functional IRC bot without PBMP, BotWeb, BotAI, or any other external Ploos service. PBMP management, BotWeb integration and BotAI capabilities are optional adapters/features only. Their absence, failure, or removal MUST NOT prevent normal IRC operation or make core bot functionality depend on an external control plane or AI service.

The web UI is deliberately out of process. Management interoperability is defined by the separate PBMP project.

License: MIT.


M1.7 adds optional native-C BotAI v1 integration. Configure `botai_url` to enable `!ai <message>`; without BotAI or libcurl LuCa remains a normal standalone IRC/Lua bot. See `docs/M1.7-BOTAI.md`.

M1.8 adds bounded, memory-only caller-owned BotAI history, `!aireset`, and opt-in natural PM/nick-addressed conversation. See `docs/M1.8-BOTAI-CONTEXT.md`.
