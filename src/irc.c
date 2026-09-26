#include "luca/irc.h"
#include <string.h>
#include <ctype.h>
int luca_irc_parse(const char *line, struct luca_irc_message *m){
 if(!line||!m) return -1; memset(m,0,sizeof *m); const char *p=line;
 if(*p==':'){ const char *e=strchr(++p,' '); if(!e) return -1; size_t n=(size_t)(e-p); if(n>=sizeof m->prefix) return -1; memcpy(m->prefix,p,n); p=e+1; }
 while(*p==' ') p++; const char *e=p; while(*e && *e!=' ' && *e!='\r' && *e!='\n') e++; size_t n=(size_t)(e-p); if(!n||n>=sizeof m->command) return -1; memcpy(m->command,p,n); p=e;
 while(*p && *p!='\r' && *p!='\n' && m->nparams<LUCA_IRC_MAX_PARAMS){ while(*p==' ') p++; if(!*p||*p=='\r'||*p=='\n') break; char *dst=m->params[m->nparams++]; if(*p==':'){ p++; e=p+strcspn(p,"\r\n"); } else e=p+strcspn(p," \r\n"); n=(size_t)(e-p); if(n>=512) n=511; memcpy(dst,p,n); dst[n]=0; p=e; if(*(p-n-1)==':') break; }
 return 0;
}

static unsigned char fold(unsigned char c){if(c>='A'&&c<='Z')return c+('a'-'A');if(c=='[')return '{';if(c==']')return '}';if(c=='\\')return '|';if(c=='^')return '~';return c;}
int luca_irc_casecmp(const char*a,const char*b){if(!a||!b)return a?1:b?-1:0;while(*a&&*b){unsigned char x=fold((unsigned char)*a++),y=fold((unsigned char)*b++);if(x!=y)return x<y?-1:1;}return *a?1:*b?-1:0;}
int luca_irc_prefix_is_nick(const char*prefix,const char*nick){if(!prefix||!nick)return 0;char p[256];size_t n=strcspn(prefix,"!@");if(n>=sizeof p)return 0;memcpy(p,prefix,n);p[n]=0;return luca_irc_casecmp(p,nick)==0;}
