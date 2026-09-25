#include "luca/sasl.h"
#include <openssl/evp.h>
#include <string.h>
#include <stdlib.h>
int luca_sasl_plain(const char*u,const char*p,char*out,size_t z){size_t a=strlen(u),b=strlen(p),n=a*2+b+2;unsigned char*raw=malloc(n);if(!raw)return-1;size_t i=0;raw[i++]=0;memcpy(raw+i,u,a);i+=a;raw[i++]=0;memcpy(raw+i,u,a);i+=a;memcpy(raw+i,p,b);i+=b;size_t need=4*((i+2)/3);if(z<=need){free(raw);return-1;}EVP_EncodeBlock((unsigned char*)out,raw,(int)i);free(raw);return 0;}
