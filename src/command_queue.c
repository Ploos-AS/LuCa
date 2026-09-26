#include "luca/command_queue.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
int luca_command_queue_init(struct luca_command_queue*q){memset(q,0,sizeof *q);q->wake_read=q->wake_write=-1;int p[2];if(pipe(p))return-1;q->wake_read=p[0];q->wake_write=p[1];fcntl(q->wake_read,F_SETFL,fcntl(q->wake_read,F_GETFL,0)|O_NONBLOCK);fcntl(q->wake_write,F_SETFL,fcntl(q->wake_write,F_GETFL,0)|O_NONBLOCK);if(pthread_mutex_init(&q->mu,NULL)){close(p[0]);close(p[1]);return-1;}return 0;}
void luca_command_queue_destroy(struct luca_command_queue*q){if(q->wake_read>=0)close(q->wake_read);if(q->wake_write>=0)close(q->wake_write);pthread_mutex_destroy(&q->mu);}
int luca_command_enqueue(struct luca_command_queue*q,const char*line){if(!q||!line||strlen(line)>=sizeof q->items[0].line)return-1;pthread_mutex_lock(&q->mu);if(q->count==LUCA_CMD_QUEUE_CAP){pthread_mutex_unlock(&q->mu);return-1;}q->items[q->tail].type=LUCA_COMMAND_IRC;snprintf(q->items[q->tail].line,sizeof q->items[q->tail].line,"%s",line);q->tail=(q->tail+1)%LUCA_CMD_QUEUE_CAP;q->count++;pthread_mutex_unlock(&q->mu);char w=1;(void)write(q->wake_write,&w,1);return 0;}
int luca_command_enqueue_reload(struct luca_command_queue*q){if(!q)return-1;pthread_mutex_lock(&q->mu);if(q->count==LUCA_CMD_QUEUE_CAP){pthread_mutex_unlock(&q->mu);return-1;}q->items[q->tail].type=LUCA_COMMAND_MODULE_RELOAD;q->items[q->tail].line[0]=0;q->tail=(q->tail+1)%LUCA_CMD_QUEUE_CAP;q->count++;pthread_mutex_unlock(&q->mu);char w=1;(void)write(q->wake_write,&w,1);return 0;}
int luca_command_wake_fd(struct luca_command_queue*q){return q?q->wake_read:-1;}
void luca_command_drain_wake(struct luca_command_queue*q){if(!q)return;char b[64];while(read(q->wake_read,b,sizeof b)>0){}}
int luca_command_dequeue(struct luca_command_queue*q,struct luca_command*out){if(!q||!out)return-1;pthread_mutex_lock(&q->mu);if(!q->count){pthread_mutex_unlock(&q->mu);return 1;}*out=q->items[q->head];q->head=(q->head+1)%LUCA_CMD_QUEUE_CAP;q->count--;pthread_mutex_unlock(&q->mu);return 0;}
