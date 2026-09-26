#include "luca/botai.h"
#include <assert.h>
#include <string.h>

int main(void){
 struct luca_botai_history h;
 struct luca_botai b;

 /* Bounded history: odd limits round down and the hard cap is 20. */
 luca_botai_history_init(&h,3);
 assert(h.limit==2);
 luca_botai_history_add(&h,"u1","a1");
 luca_botai_history_add(&h,"u2","a2");
 assert(h.count==2);
 assert(!strcmp(h.items[0].role,"user")&&!strcmp(h.items[0].content,"u2"));
 assert(!strcmp(h.items[1].role,"assistant")&&!strcmp(h.items[1].content,"a2"));
 luca_botai_history_reset(&h);
 assert(h.count==0);
 luca_botai_history_init(&h,99);
 assert(h.limit==20);

 /* BotAI is optional: missing/empty configuration fails locally. */
 assert(luca_botai_init(&b,NULL,"auto",100)==-1);
 assert(luca_botai_init(&b,"","auto",100)==-1);

#ifdef LUCA_HAVE_CURL
 /* An unreachable BotAI endpoint must fail cleanly and remain bounded by timeout. */
 assert(luca_botai_init(&b,"http://127.0.0.1:1","auto",100)==0);
 assert(luca_botai_compatible(&b)==-1);
 {
  char reply[64];
  assert(luca_botai_chat(&b,"hello",reply,sizeof reply)==-1);
 }
#endif

 return 0;
}
