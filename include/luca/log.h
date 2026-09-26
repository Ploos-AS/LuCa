#ifndef LUCA_LOG_H
#define LUCA_LOG_H
#include <pthread.h>
#define LUCA_LOG_CAP 32
#define LUCA_LOG_MSG 192
struct luca_log_entry { char level[8]; char message[LUCA_LOG_MSG]; };
struct luca_log { pthread_mutex_t mu; struct luca_log_entry entries[LUCA_LOG_CAP]; unsigned next,count; };
int luca_log_init(struct luca_log *l);
void luca_log_destroy(struct luca_log *l);
void luca_log_add(struct luca_log *l,const char *level,const char *message);
unsigned luca_log_snapshot(struct luca_log *l,struct luca_log_entry *out,unsigned cap);
#endif
