#ifndef LUCA_TRANSPORT_H
#define LUCA_TRANSPORT_H
#include <stddef.h>
struct luca_transport;
struct luca_transport *luca_transport_connect(const char *host,const char *port,int tls);
int luca_transport_write(struct luca_transport *,const char *,size_t);
int luca_transport_readline(struct luca_transport *,char *,size_t);
void luca_transport_close(struct luca_transport *);
#endif
