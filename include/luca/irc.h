#ifndef LUCA_IRC_H
#define LUCA_IRC_H
#include <stddef.h>
#define LUCA_IRC_MAX_PARAMS 15
struct luca_irc_message { char prefix[256]; char command[32]; char params[LUCA_IRC_MAX_PARAMS][512]; size_t nparams; };
int luca_irc_parse(const char *line, struct luca_irc_message *out);
#endif
