#ifndef LUCA_PBMP_H
#define LUCA_PBMP_H
#include <stddef.h>
struct luca_pbmp_state { const char *nick; const char *network; const char *channel; int connected; };
int luca_pbmp_response(const char *request,const struct luca_pbmp_state *,char *out,size_t outsz);
int luca_pbmp_serve(const char *socket_path,const struct luca_pbmp_state *);
#endif
