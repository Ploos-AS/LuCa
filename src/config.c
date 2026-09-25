#include "luca/config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static void set(char*d,size_t n,const char*s){snprintf(d,n,"%s",s);} static int yes(const char*s){return !strcmp(s,"1")||!strcmp(s,"true")||!strcmp(s,"yes");}
int luca_config_load(const char*path,struct luca_config*c){memset(c,0,sizeof*c);set(c->port,sizeof c->port,"6697");set(c->nick,sizeof c->nick,"LuCa");set(c->user,sizeof c->user,"luca");set(c->realname,sizeof c->realname,"LuCa IRC bot");set(c->pbmp_socket,sizeof c->pbmp_socket,"/tmp/luca.pbmp.sock");c->reconnect_seconds=5;c->tls=1;FILE*f=fopen(path,"r");if(!f)return-1;char line[512];while(fgets(line,sizeof line,f)){char*p=line;while(*p==' '||*p=='\t')p++;if(*p=='#'||*p=='\n'||!*p)continue;char*eq=strchr(p,'=');if(!eq)continue;*eq++=0;p[strcspn(p," \t")]=0;eq[strcspn(eq,"\r\n")]=0;
#define K(k,field) if(!strcmp(p,k)){set(c->field,sizeof c->field,eq);continue;}
K("server",server) K("port",port) K("nick",nick) K("user",user) K("realname",realname) K("channel",channel) K("script",script) K("sasl_user",sasl_user) K("sasl_password",sasl_password) K("pbmp_socket",pbmp_socket)
if(!strcmp(p,"reconnect_seconds"))c->reconnect_seconds=atoi(eq);else if(!strcmp(p,"tls"))c->tls=yes(eq);else if(!strcmp(p,"sasl"))c->sasl=yes(eq);
#undef K
}fclose(f);if(c->sasl&&(!c->sasl_user[0]||!c->sasl_password[0]))return-1;return c->server[0]?0:-1;}
