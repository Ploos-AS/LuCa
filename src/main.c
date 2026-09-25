#include "luca/config.h"
#include "luca/irc.h"
#include "luca/lua_runtime.h"
#include "luca/transport.h"
#include "luca/sasl.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
struct conn{struct luca_transport*t;};
static int sendline(void*o,const char*s){struct conn*c=o;return luca_transport_write(c->t,s,strlen(s))||luca_transport_write(c->t,"\r\n",2)?-1:0;}
int main(int argc,char**argv){struct luca_config cfg;if(argc!=2||luca_config_load(argv[1],&cfg)){fprintf(stderr,"usage: luca <config>\n");return 2;}for(;;){struct conn c={luca_transport_connect(cfg.server,cfg.port,cfg.tls)};if(!c.t){sleep(cfg.reconnect_seconds);continue;}char b[1024];sendline(&c,"CAP LS 302");snprintf(b,sizeof b,"NICK %s",cfg.nick);sendline(&c,b);snprintf(b,sizeof b,"USER %s 0 * :%s",cfg.user,cfg.realname);sendline(&c,b);struct luca_lua*r=luca_lua_open(cfg.script,sendline,&c);int cap_done=0,joined=0;while(luca_transport_readline(c.t,b,sizeof b)>0){struct luca_irc_message m;if(luca_irc_parse(b,&m))continue;if(!strcmp(m.command,"PING")&&m.nparams){char out[600];snprintf(out,sizeof out,"PONG :%s",m.params[0]);sendline(&c,out);}if(!strcmp(m.command,"CAP")&&m.nparams>=3&&!strcmp(m.params[1],"LS")){if(cfg.sasl&&strstr(m.params[m.nparams-1],"sasl"))sendline(&c,"CAP REQ :sasl");else{sendline(&c,"CAP END");cap_done=1;}}else if(!strcmp(m.command,"CAP")&&m.nparams>=3&&!strcmp(m.params[1],"ACK")&&strstr(m.params[m.nparams-1],"sasl")){sendline(&c,"AUTHENTICATE PLAIN");}else if(!strcmp(m.command,"AUTHENTICATE")&&m.nparams&&cfg.sasl){char enc[768],out[800];if(!luca_sasl_plain(cfg.sasl_user,cfg.sasl_password,enc,sizeof enc)){snprintf(out,sizeof out,"AUTHENTICATE %s",enc);sendline(&c,out);}}else if(!strcmp(m.command,"903")){sendline(&c,"CAP END");cap_done=1;}else if(!strcmp(m.command,"904")||!strcmp(m.command,"905")||!strcmp(m.command,"906")||!strcmp(m.command,"907")){fprintf(stderr,"SASL authentication failed\n");break;}if(cap_done&&!joined&&(!strcmp(m.command,"001")||!strcmp(m.command,"903"))){if(cfg.channel[0]){snprintf(b,sizeof b,"JOIN %s",cfg.channel);sendline(&c,b);}joined=1;}luca_lua_event(r,&m);}luca_lua_close(r);luca_transport_close(c.t);sleep(cfg.reconnect_seconds);}}
