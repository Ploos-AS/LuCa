#include "luca/log.h"
#include <stdio.h>
#include <string.h>
int luca_log_init(struct luca_log*l){memset(l,0,sizeof *l);return pthread_mutex_init(&l->mu,NULL);}
void luca_log_destroy(struct luca_log*l){pthread_mutex_destroy(&l->mu);}
void luca_log_add(struct luca_log*l,const char*level,const char*message){if(!l||!level||!message)return;pthread_mutex_lock(&l->mu);struct luca_log_entry*e=&l->entries[l->next];snprintf(e->level,sizeof e->level,"%s",level);snprintf(e->message,sizeof e->message,"%s",message);for(char*p=e->message;*p;p++)if(*p=='\r'||*p=='\n')*p=' ';l->next=(l->next+1)%LUCA_LOG_CAP;if(l->count<LUCA_LOG_CAP)l->count++;pthread_mutex_unlock(&l->mu);}
unsigned luca_log_snapshot(struct luca_log*l,struct luca_log_entry*out,unsigned cap){if(!l||!out)return 0;pthread_mutex_lock(&l->mu);unsigned n=l->count<cap?l->count:cap,start=(l->next+LUCA_LOG_CAP-l->count)%LUCA_LOG_CAP;for(unsigned i=0;i<n;i++)out[i]=l->entries[(start+i)%LUCA_LOG_CAP];pthread_mutex_unlock(&l->mu);return n;}
