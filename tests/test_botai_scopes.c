#include "luca/botai_scopes.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(void){
 struct luca_botai_scopes s;luca_botai_scopes_init(&s,10);
 struct luca_botai_history*a=luca_botai_scopes_get(&s,"channel:#a");
 struct luca_botai_history*b=luca_botai_scopes_get(&s,"nick:bob");
 assert(a&&b&&a!=b);luca_botai_history_add(a,"u","a");assert(a->count==2&&b->count==0);
 luca_botai_scopes_reset(&s,"channel:#a");assert(a->count==0);
 char key[64];for(int i=0;i<40;i++){snprintf(key,sizeof key,"nick:%d",i);assert(luca_botai_scopes_get(&s,key));}
 assert(s.count==LUCA_BOTAI_MAX_SCOPES);
 assert(luca_botai_scopes_get(&s,"nick:39")->limit==10);
 return 0;
}
