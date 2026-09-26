#include "luca/botai.h"
#include <stdio.h>
#include <string.h>
#ifdef LUCA_HAVE_CURL
#include <curl/curl.h>
struct sink{char*p;size_t n,cap;};
static size_t wr(char*p,size_t s,size_t n,void*v){struct sink*x=v;size_t z=s*n;if(x->n+z>=x->cap)return 0;memcpy(x->p+x->n,p,z);x->n+=z;x->p[x->n]=0;return z;}
static void esc(const char*s,char*d,size_t n){size_t j=0;for(;*s&&j+2<n;s++){if(*s=='"'||*s=='\\')d[j++]='\\';if((unsigned char)*s>=32)d[j++]=*s;}d[j]=0;}
static int req(struct luca_botai*b,const char*path,const char*body,char*out,size_t n){CURL*c=curl_easy_init();if(!c)return-1;char u[512];snprintf(u,sizeof u,"%s%s",b->base_url,path);struct sink x={out,0,n};out[0]=0;curl_easy_setopt(c,CURLOPT_URL,u);curl_easy_setopt(c,CURLOPT_TIMEOUT_MS,b->timeout_ms);curl_easy_setopt(c,CURLOPT_WRITEFUNCTION,wr);curl_easy_setopt(c,CURLOPT_WRITEDATA,&x);struct curl_slist*h=NULL;if(body){h=curl_slist_append(h,"Content-Type: application/json");curl_easy_setopt(c,CURLOPT_HTTPHEADER,h);curl_easy_setopt(c,CURLOPT_POSTFIELDS,body);}CURLcode e=curl_easy_perform(c);long code=0;curl_easy_getinfo(c,CURLINFO_RESPONSE_CODE,&code);curl_slist_free_all(h);curl_easy_cleanup(c);return e==CURLE_OK&&code>=200&&code<300?0:-1;}
#endif
int luca_botai_init(struct luca_botai*b,const char*url,const char*expert,long timeout_ms){if(!b||!url||!*url)return-1;memset(b,0,sizeof*b);snprintf(b->base_url,sizeof b->base_url,"%s",url);size_t z=strlen(b->base_url);while(z&&b->base_url[z-1]=='/')b->base_url[--z]=0;snprintf(b->expert,sizeof b->expert,"%s",expert&&*expert?expert:"auto");b->timeout_ms=timeout_ms>0?timeout_ms:30000;
#ifdef LUCA_HAVE_CURL
b->available=1;return 0;
#else
return-1;
#endif
}
int luca_botai_compatible(struct luca_botai*b){
#ifdef LUCA_HAVE_CURL
char out[1024];if(!b||!b->available||req(b,"/v1/version",NULL,out,sizeof out))return-1;return strstr(out,"\"api_version\":\"1.0.0\"")?0:-1;
#else
(void)b;return-1;
#endif
}
int luca_botai_chat(struct luca_botai*b,const char*message,char*reply,size_t reply_size){
#ifdef LUCA_HAVE_CURL
if(!b||!message||!*message||!reply||reply_size<2)return-1;char em[4096],ee[256],body[4608],out[16384];esc(message,em,sizeof em);esc(b->expert,ee,sizeof ee);snprintf(body,sizeof body,"{\"expert\":\"%s\",\"message\":\"%s\"}",ee,em);if(req(b,"/v1/chat",body,out,sizeof out))return-1;char*p=strstr(out,"\"text\":");if(!p)return-1;p=strchr(p,':');if(!p)return-1;p++;while(*p==' ')*p++;if(*p++!='"')return-1;size_t j=0;while(*p&&*p!='"'&&j+1<reply_size){if(*p=='\\'&&p[1]){p++;if(*p=='n')reply[j++]=' ';else reply[j++]=*p++;}else reply[j++]=*p++;}reply[j]=0;return j?0:-1;
#else
(void)b;(void)message;(void)reply;(void)reply_size;return-1;
#endif
}
