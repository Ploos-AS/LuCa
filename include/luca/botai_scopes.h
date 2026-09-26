#ifndef LUCA_BOTAI_SCOPES_H
#define LUCA_BOTAI_SCOPES_H
#include "luca/botai.h"
#include <stddef.h>
#define LUCA_BOTAI_MAX_SCOPES 32
struct luca_botai_scope { char key[160]; unsigned long age; struct luca_botai_history history; };
struct luca_botai_scopes { struct luca_botai_scope items[LUCA_BOTAI_MAX_SCOPES]; size_t count; size_t history_limit; unsigned long clock; };
void luca_botai_scopes_init(struct luca_botai_scopes *s,size_t history_limit);
struct luca_botai_history *luca_botai_scopes_get(struct luca_botai_scopes *s,const char *key);
void luca_botai_scopes_reset(struct luca_botai_scopes *s,const char *key);
#endif
