# PBMP in LuCa

LuCa M0.2 implements the required PBMP/1 discovery methods over a local Unix domain stream socket. The default is `/tmp/luca.pbmp.sock` and may be changed with `pbmp_socket`.

The socket is created mode 0600. No TCP listener is provided. This keeps the initial management surface local-only while BotWeb and Engo gain independent PBMP implementations.

Implemented methods: `pbmp.info`, `capabilities.list`, `bot.info`, and `networks.list`.

One newline-delimited JSON request is accepted per connection in M0.2. Persistent sessions and event streaming are later milestones.
