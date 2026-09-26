#ifndef LUCA_PBMP_H
#define LUCA_PBMP_H
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <stdatomic.h>
#include "luca/command_queue.h"
#include "luca/log.h"
#include <pthread.h>
#define LUCA_CHANNEL_CAP 16
struct luca_channel_entry { char name[201]; int wanted,joined,parting; };
struct luca_channel_registry { pthread_mutex_t mu; struct luca_channel_entry items[LUCA_CHANNEL_CAP]; unsigned count; };
int luca_channel_registry_init(struct luca_channel_registry *r,const char *configured);
void luca_channel_registry_set(struct luca_channel_registry *r,const char *name,int wanted,int joined,int parting);
void luca_channel_registry_observe(struct luca_channel_registry *r,const char *name,int joined);

struct luca_pbmp_state { const char *nick; const char *network; const char *channel; atomic_int connected; atomic_int channel_state; struct luca_command_queue *commands; const char *module_id; atomic_int module_active; atomic_uint_fast64_t rx_lines; atomic_uint_fast64_t tx_lines; atomic_uint_fast64_t reconnects; atomic_long session_started; struct luca_log *log; const char *port; const char *user; const char *realname; const char *script; int tls; int sasl; int reconnect_seconds; struct luca_channel_registry *channels; };
int luca_pbmp_response(const char *request,const struct luca_pbmp_state *,char *out,size_t outsz);
int luca_pbmp_serve(const char *socket_path,const struct luca_pbmp_state *);
#endif
