#ifndef LUCA_LUA_RUNTIME_H
#define LUCA_LUA_RUNTIME_H
#include "luca/irc.h"
typedef int (*luca_send_fn)(void *, const char *);
struct luca_lua;
struct luca_lua *luca_lua_open(const char *script, luca_send_fn send_fn, void *opaque);
void luca_lua_event(struct luca_lua *, const struct luca_irc_message *);
void luca_lua_close(struct luca_lua *);
#endif
