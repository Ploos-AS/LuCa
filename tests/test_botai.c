#include "luca/botai.h"
#include <assert.h>
#include <string.h>
#ifdef LUCA_HAVE_CURL
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define CHECK(x) do { if(!(x)) abort(); } while(0)

struct fixture { int fd; int port; const char *response; };
static void *serve_once(void *arg){
 struct fixture *f=arg; int c=accept(f->fd,NULL,NULL); CHECK(c>=0);
 char req[4096]; (void)read(c,req,sizeof req);
 size_t n=strlen(f->response), off=0;
 while(off<n){ ssize_t z=write(c,f->response+off,n-off); if(z<=0)break; off+=(size_t)z; }
 close(c); close(f->fd); return NULL;
}
static pthread_t fixture_start(struct fixture *f,const char *response){
 f->fd=socket(AF_INET,SOCK_STREAM,0); CHECK(f->fd>=0);
 struct sockaddr_in a={0}; a.sin_family=AF_INET; a.sin_addr.s_addr=htonl(INADDR_LOOPBACK); a.sin_port=0;
 CHECK(bind(f->fd,(struct sockaddr*)&a,sizeof a)==0); CHECK(listen(f->fd,1)==0);
 socklen_t z=sizeof a; CHECK(getsockname(f->fd,(struct sockaddr*)&a,&z)==0); f->port=ntohs(a.sin_port); f->response=response;
 pthread_t t; CHECK(pthread_create(&t,NULL,serve_once,f)==0); return t;
}
static void fixture_url(const struct fixture *f,char *url,size_t n){snprintf(url,n,"http://127.0.0.1:%d",f->port);}
static void check_compat(const char *response,int expected){
 struct fixture f; pthread_t t=fixture_start(&f,response); char url[64]; fixture_url(&f,url,sizeof url);
 struct luca_botai b; CHECK(luca_botai_init(&b,url,"auto",1000)==0); CHECK(luca_botai_compatible(&b)==expected);
 pthread_join(t,NULL);
}
#endif

int main(void){
 struct luca_botai_history h; struct luca_botai b;
 luca_botai_history_init(&h,3); CHECK(h.limit==2);
 luca_botai_history_add(&h,"u1","a1"); luca_botai_history_add(&h,"u2","a2");
 CHECK(h.count==2); CHECK(!strcmp(h.items[0].content,"u2")); CHECK(!strcmp(h.items[1].content,"a2"));
 luca_botai_history_reset(&h); CHECK(h.count==0); luca_botai_history_init(&h,99); CHECK(h.limit==20);
 CHECK(luca_botai_init(&b,NULL,"auto",100)==-1); CHECK(luca_botai_init(&b,"","auto",100)==-1);
#ifdef LUCA_HAVE_CURL
 CHECK(luca_botai_init(&b,"http://127.0.0.1:1","auto",100)==0); CHECK(luca_botai_compatible(&b)==-1);
 { char reply[64]; CHECK(luca_botai_chat(&b,"hello",reply,sizeof reply)==-1); }
 check_compat("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 23\r\nConnection: close\r\n\r\n{\"api_version\":\"1.0.0\"}",0);
 check_compat("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 23\r\nConnection: close\r\n\r\n{\"api_version\":\"2.0.0\"}",-1);
 check_compat("HTTP/1.1 500 Internal Server Error\r\nContent-Length: 14\r\nConnection: close\r\n\r\nprovider secret",-1);
 check_compat("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 9\r\nConnection: close\r\n\r\n{not-json",-1);
#endif
 return 0;
}
