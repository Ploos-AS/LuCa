#ifndef LUCA_BOTAI_H
#define LUCA_BOTAI_H
#include <stddef.h>
#define LUCA_BOTAI_API_VERSION "1.0.0"
#define LUCA_BOTAI_MAX_HISTORY 20
struct luca_botai_message { char role[10]; char content[1024]; };
struct luca_botai_history { struct luca_botai_message items[LUCA_BOTAI_MAX_HISTORY]; size_t count; size_t limit; };
struct luca_botai { char base_url[256]; char expert[64]; long timeout_ms; int available; };
int luca_botai_init(struct luca_botai *b,const char *url,const char *expert,long timeout_ms);
int luca_botai_compatible(struct luca_botai *b);
int luca_botai_chat(struct luca_botai *b,const char *message,char *reply,size_t reply_size);
int luca_botai_chat_history(struct luca_botai *b,const struct luca_botai_history *h,const char *message,char *reply,size_t reply_size);
void luca_botai_history_init(struct luca_botai_history *h,size_t limit);
void luca_botai_history_reset(struct luca_botai_history *h);
void luca_botai_history_add(struct luca_botai_history *h,const char *user,const char *assistant);
#endif
