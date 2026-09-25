#include "luca/sasl.h"
#include <assert.h>
#include <string.h>
int main(void){char b[128];assert(luca_sasl_plain("user","pass",b,sizeof b)==0);assert(!strcmp(b,"AHVzZXIAdXNlcnBhc3M="));return 0;}
