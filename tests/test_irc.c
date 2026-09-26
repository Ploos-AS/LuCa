#include "luca/irc.h"
#include <assert.h>
#include <string.h>
int main(void){struct luca_irc_message m;assert(luca_irc_parse(":nick!u@h PRIVMSG #luca :hello world\r\n",&m)==0);assert(!strcmp(m.prefix,"nick!u@h"));assert(!strcmp(m.command,"PRIVMSG"));assert(m.nparams==2);assert(!strcmp(m.params[0],"#luca"));assert(!strcmp(m.params[1],"hello world"));assert(luca_irc_parse("PING :abc\r\n",&m)==0);assert(!strcmp(m.params[0],"abc"));assert(luca_irc_casecmp("Nick[One]\\\\^","nICK{one}|~")==0);assert(luca_irc_prefix_is_nick("Nick[One]!user@host","nick{one}"));assert(!luca_irc_prefix_is_nick("NickExtra!u@h","Nick"));return 0;}
