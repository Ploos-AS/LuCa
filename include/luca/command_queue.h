#ifndef LUCA_COMMAND_QUEUE_H
#define LUCA_COMMAND_QUEUE_H
#include <pthread.h>
#define LUCA_CMD_QUEUE_CAP 16
struct luca_command { char line[512]; };
struct luca_command_queue { pthread_mutex_t mu; struct luca_command items[LUCA_CMD_QUEUE_CAP]; unsigned head, tail, count; };
int luca_command_queue_init(struct luca_command_queue *q);
void luca_command_queue_destroy(struct luca_command_queue *q);
int luca_command_enqueue(struct luca_command_queue *q,const char *line);
int luca_command_dequeue(struct luca_command_queue *q,char *out,unsigned n);
#endif
