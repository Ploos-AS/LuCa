#ifndef LUCA_CONFIG_H
#define LUCA_CONFIG_H
struct luca_config { char server[256]; char port[16]; char nick[64]; char user[64]; char realname[128]; char channel[128]; char script[256]; int reconnect_seconds; };
int luca_config_load(const char *path, struct luca_config *cfg);
#endif
