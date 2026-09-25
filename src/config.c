#include "luca/config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static void set(char *d,size_t n,const char *s){ snprintf(d,n,"%s",s); }
int luca_config_load(const char *path, struct luca_config *c){
 memset(c,0,sizeof *c); set(c->port,sizeof c->port,"6667"); set(c->nick,sizeof c->nick,"LuCa"); set(c->user,sizeof c->user,"luca"); set(c->realname,sizeof c->realname,"LuCa IRC bot"); c->reconnect_seconds=5;
 FILE *f=fopen(path,"r"); if(!f) return -1; char line[512]; while(fgets(line,sizeof line,f)){ char *p=line; while(*p==' '||*p=='\t')p++; if(*p=='#'||*p=='\n'||!*p)continue; char *eq=strchr(p,'='); if(!eq)continue; *eq++=0; p[strcspn(p," \t")]=0; eq[strcspn(eq,"\r\n")]=0;
 #define K(k,field) if(!strcmp(p,k)){set(c->field,sizeof c->field,eq);continue;}
 K("server",server) K("port",port) K("nick",nick) K("user",user) K("realname",realname) K("channel",channel) K("script",script)
 if(!strcmp(p,"reconnect_seconds")) c->reconnect_seconds=atoi(eq);
 #undef K
 } fclose(f); return c->server[0]?0:-1;
}
