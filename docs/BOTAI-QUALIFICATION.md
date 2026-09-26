# BotAI integration qualification

LuCa's optional native-C BotAI v1 adapter is qualified against the standalone-first integration contract.

## Qualified commit

- LuCa: `9c3ecada281776e428752bd68d1a8ba8634cc843`
- GitHub Actions run: `36273087495`
- Result: **PASS**

## Coverage

The qualification verifies:

- LuCa remains valid with BotAI absent or disabled.
- BotAI configuration rejects missing/empty endpoints locally.
- Unreachable BotAI fails cleanly with a finite timeout.
- API `1.0.0` is accepted.
- An incompatible API version is rejected.
- HTTP error responses fail without exposing the provider response body through the adapter API.
- Malformed version responses are rejected.
- Conversation history remains bounded to 20 messages.
- BotAI responses are bounded to 16 KiB.
- The adapter and tests build and pass in the normal Ubuntu CI environment.
- The adapter and tests build and pass in the Alpine OCI Release build.

The BotAI integration remains optional. IRC, Lua scripting, PBMP and the rest of the LuCa core do not require BotAI at runtime.
