#define _POSIX_C_SOURCE 200112L
#include "luca/config.h"
#include "luca/irc.h"
#include "luca/lua_runtime.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
struct conn { int fd; };
static int sendline(void *o,const char *s){struct conn*c=o;size_t n=strlen(s);return send(c->fd,s,n,0)==(ssize_t)n&&send(c->fd,"\r\n",2,0)==2?0:-1;}
static int connect_irc(const struct luca_config*c){struct addrinfo h={0},*r,*p;h.ai_socktype=SOCK_STREAM;h.ai_family=AF_UNSPEC;if(getaddrinfo(c->server,c->port,&h,&r))return -1;int fd=-1;for(p=r;p;p=p->ai_next){fd=socket(p->ai_family,p->ai_socktype,p->ai_protocol);if(fd>=0&&connect(fd,p->ai_addr,p->ai_addrlen)==0)break;if(fd>=0)close(fd);fd=-1;}freeaddrinfo(r);return fd;}
int main(int argc,char**argv){struct luca_config cfg;if(argc!=2||luca_config_load(argv[1],&cfg)){fprintf(stderr,"usage: luca <config>\n");return 2;}for(;;){struct conn c={connect_irc(&cfg)};if(c.fd<0){sleep(cfg.reconnect_seconds);continue;}char b[1024];snprintf(b,sizeof b,"NICK %s",cfg.nick);sendline(&c,b);snprintf(b,sizeof b,"USER %s 0 * :%s",cfg.user,cfg.realname);sendline(&c,b);if(cfg.channel[0]){snprintf(b,sizeof b,"JOIN %s",cfg.channel);sendline(&c,b);}struct luca_lua*r=luca_lua_open(cfg.script,sendline,&c);FILE*f=fdopen(c.fd,"r");while(f&&fgets(b,sizeof b,f)){struct luca_irc_message m;if(!luca_irc_parse(b,&m)){if(!strcmp(m.command,"PING")&&m.nparams){char out[600];snprintf(out,sizeof out,"PONG :%s",m.params[0]);sendline(&c,out);}luca_lua_event(r,&m);}}luca_lua_close(r);if(f)fclose(f);else close(c.fd);sleep(cfg.reconnect_seconds);} }
