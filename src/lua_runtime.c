#include "luca/lua_runtime.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdlib.h>
struct luca_lua { lua_State *L; luca_send_fn send; void *opaque; };
static int l_send(lua_State *L){ struct luca_lua *r=*(struct luca_lua **)lua_getextraspace(L); const char *s=luaL_checkstring(L,1); lua_pushboolean(L,r->send(r->opaque,s)==0); return 1; }
struct luca_lua *luca_lua_open(const char *script,luca_send_fn fn,void *opaque){ struct luca_lua *r=calloc(1,sizeof *r); if(!r)return NULL; r->L=luaL_newstate(); r->send=fn;r->opaque=opaque; if(!r->L){free(r);return NULL;} *(struct luca_lua **)lua_getextraspace(r->L)=r; luaL_openlibs(r->L); lua_newtable(r->L); lua_pushcfunction(r->L,l_send);lua_setfield(r->L,-2,"send");lua_setglobal(r->L,"luca"); if(script&&*script&&luaL_dofile(r->L,script)!=LUA_OK){lua_close(r->L);free(r);return NULL;} return r; }
void luca_lua_event(struct luca_lua *r,const struct luca_irc_message *m){ if(!r)return; lua_getglobal(r->L,"on_irc"); if(!lua_isfunction(r->L,-1)){lua_pop(r->L,1);return;} lua_newtable(r->L); lua_pushstring(r->L,m->prefix);lua_setfield(r->L,-2,"prefix");lua_pushstring(r->L,m->command);lua_setfield(r->L,-2,"command");lua_newtable(r->L);for(size_t i=0;i<m->nparams;i++){lua_pushstring(r->L,m->params[i]);lua_rawseti(r->L,-2,(lua_Integer)i+1);}lua_setfield(r->L,-2,"params"); if(lua_pcall(r->L,1,0,0)!=LUA_OK) lua_pop(r->L,1); }
void luca_lua_close(struct luca_lua *r){if(r){if(r->L)lua_close(r->L);free(r);}}
