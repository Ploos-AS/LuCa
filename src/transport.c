#define _POSIX_C_SOURCE 200112L
#include "luca/transport.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>
struct luca_transport{int fd,tls;SSL_CTX*ctx;SSL*ssl;};
struct luca_transport*luca_transport_connect(const char*h,const char*p,int tls){struct addrinfo q={0},*r,*a;q.ai_socktype=SOCK_STREAM;q.ai_family=AF_UNSPEC;if(getaddrinfo(h,p,&q,&r))return NULL;int fd=-1;for(a=r;a;a=a->ai_next){fd=socket(a->ai_family,a->ai_socktype,a->ai_protocol);if(fd>=0&&connect(fd,a->ai_addr,a->ai_addrlen)==0)break;if(fd>=0)close(fd);fd=-1;}freeaddrinfo(r);if(fd<0)return NULL;struct luca_transport*t=calloc(1,sizeof*t);if(!t){close(fd);return NULL;}t->fd=fd;t->tls=tls;if(!tls)return t;t->ctx=SSL_CTX_new(TLS_client_method());if(!t->ctx)goto fail;SSL_CTX_set_min_proto_version(t->ctx,TLS1_2_VERSION);SSL_CTX_set_verify(t->ctx,SSL_VERIFY_PEER,NULL);if(!SSL_CTX_set_default_verify_paths(t->ctx))goto fail;t->ssl=SSL_new(t->ctx);if(!t->ssl||!SSL_set_tlsext_host_name(t->ssl,h)||!SSL_set1_host(t->ssl,h)||SSL_set_fd(t->ssl,fd)!=1||SSL_connect(t->ssl)!=1)goto fail;return t;fail:luca_transport_close(t);return NULL;}
int luca_transport_write(struct luca_transport*t,const char*b,size_t n){size_t off=0;while(off<n){int x=t->tls?SSL_write(t->ssl,b+off,(int)(n-off)):(int)send(t->fd,b+off,n-off,0);if(x<=0)return-1;off+=(size_t)x;}return 0;}
int luca_transport_wait(struct luca_transport*t,int wake_fd){struct pollfd p[2]={{t->fd,POLLIN,0},{wake_fd,POLLIN,0}};int r=poll(p,2,-1);if(r<=0)return-1;if(p[1].revents&POLLIN)return 1;if(p[0].revents&(POLLIN|POLLHUP|POLLERR))return 0;return-1;}
int luca_transport_readline(struct luca_transport*t,char*b,size_t n){size_t i=0;if(!n)return-1;while(i+1<n){char c;int x=t->tls?SSL_read(t->ssl,&c,1):(int)recv(t->fd,&c,1,0);if(x<=0)return-1;b[i++]=c;if(c=='\n')break;}b[i]=0;return(int)i;}
void luca_transport_close(struct luca_transport*t){if(!t)return;if(t->ssl){SSL_shutdown(t->ssl);SSL_free(t->ssl);}if(t->ctx)SSL_CTX_free(t->ctx);if(t->fd>=0)close(t->fd);free(t);}
