#ifndef LUCA_SASL_H
#define LUCA_SASL_H
#include <stddef.h>
int luca_sasl_plain(const char *user,const char *pass,char *out,size_t outsz);
#endif
