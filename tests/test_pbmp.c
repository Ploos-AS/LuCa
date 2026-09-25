#include "luca/pbmp.h"
#include <assert.h>
#include <string.h>
int main(void){struct luca_pbmp_state s={"LuCa","irc.example",1};char b[2048];assert(luca_pbmp_response("{\"pbmp\":1,\"type\":\"request\",\"id\":\"42\",\"method\":\"bot.info\",\"params\":{}}",&s,b,sizeof b)==0);assert(strstr(b,"\"id\":\"42\""));assert(strstr(b,"\"implementation\":\"luca\""));assert(strstr(b,"\"state\":\"online\""));assert(luca_pbmp_response("{\"pbmp\":1,\"type\":\"request\",\"id\":\"9\",\"method\":\"x\",\"params\":{}}",&s,b,sizeof b)==0);assert(strstr(b,"not_supported"));return 0;}
