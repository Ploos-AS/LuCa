#ifndef LUCA_BOTAI_H
#define LUCA_BOTAI_H
#include <stddef.h>
#define LUCA_BOTAI_API_VERSION "1.0.0"
struct luca_botai { char base_url[256]; char expert[64]; long timeout_ms; int available; };
int luca_botai_init(struct luca_botai *b,const char *url,const char *expert,long timeout_ms);
int luca_botai_compatible(struct luca_botai *b);
int luca_botai_chat(struct luca_botai *b,const char *message,char *reply,size_t reply_size);
#endif
