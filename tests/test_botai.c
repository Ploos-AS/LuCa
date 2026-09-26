#include "luca/botai.h"
#include <assert.h>
#include <string.h>
int main(void){
 struct luca_botai_history h;
 luca_botai_history_init(&h,3);
 assert(h.limit==2);
 luca_botai_history_add(&h,"u1","a1");
 luca_botai_history_add(&h,"u2","a2");
 assert(h.count==2);
 assert(!strcmp(h.items[0].role,"user")&&!strcmp(h.items[0].content,"u2"));
 assert(!strcmp(h.items[1].role,"assistant")&&!strcmp(h.items[1].content,"a2"));
 luca_botai_history_reset(&h);assert(h.count==0);
 luca_botai_history_init(&h,99);assert(h.limit==20);
 return 0;
}
