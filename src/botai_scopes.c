#include "luca/botai_scopes.h"
#include <string.h>
#include <stdio.h>
void luca_botai_scopes_init(struct luca_botai_scopes*s,size_t limit){if(!s)return;memset(s,0,sizeof*s);s->history_limit=limit;}
struct luca_botai_history*luca_botai_scopes_get(struct luca_botai_scopes*s,const char*key){if(!s||!key||!*key)return NULL;s->clock++;for(size_t i=0;i<s->count;i++)if(!strcmp(s->items[i].key,key)){s->items[i].age=s->clock;return&s->items[i].history;}size_t i;if(s->count<LUCA_BOTAI_MAX_SCOPES)i=s->count++;else{unsigned long oldest=s->items[0].age;i=0;for(size_t j=1;j<s->count;j++)if(s->items[j].age<oldest){oldest=s->items[j].age;i=j;}}memset(&s->items[i],0,sizeof s->items[i]);snprintf(s->items[i].key,sizeof s->items[i].key,"%s",key);s->items[i].age=s->clock;luca_botai_history_init(&s->items[i].history,s->history_limit);return&s->items[i].history;}
void luca_botai_scopes_reset(struct luca_botai_scopes*s,const char*key){if(!s||!key)return;for(size_t i=0;i<s->count;i++)if(!strcmp(s->items[i].key,key)){luca_botai_history_reset(&s->items[i].history);return;}}
