#include "luca/command_queue.h"
#include <stdio.h>
#include <string.h>
int luca_command_queue_init(struct luca_command_queue*q){memset(q,0,sizeof *q);return pthread_mutex_init(&q->mu,NULL);}
void luca_command_queue_destroy(struct luca_command_queue*q){pthread_mutex_destroy(&q->mu);}
int luca_command_enqueue(struct luca_command_queue*q,const char*line){if(!q||!line||strlen(line)>=sizeof q->items[0].line)return-1;pthread_mutex_lock(&q->mu);if(q->count==LUCA_CMD_QUEUE_CAP){pthread_mutex_unlock(&q->mu);return-1;}snprintf(q->items[q->tail].line,sizeof q->items[q->tail].line,"%s",line);q->tail=(q->tail+1)%LUCA_CMD_QUEUE_CAP;q->count++;pthread_mutex_unlock(&q->mu);return 0;}
int luca_command_dequeue(struct luca_command_queue*q,char*out,unsigned n){if(!q||!out||!n)return-1;pthread_mutex_lock(&q->mu);if(!q->count){pthread_mutex_unlock(&q->mu);return 1;}snprintf(out,n,"%s",q->items[q->head].line);q->head=(q->head+1)%LUCA_CMD_QUEUE_CAP;q->count--;pthread_mutex_unlock(&q->mu);return 0;}
