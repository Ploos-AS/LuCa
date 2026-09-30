#include "luca/pbmp.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    const char *socket_path = getenv("LUCA_PBMP_SOCKET");
    if (!socket_path || !*socket_path) {
        fputs("LUCA_PBMP_SOCKET is required\n", stderr);
        return 2;
    }
    struct luca_pbmp_state state = {0};
    state.nick = "luca-qualification";
    state.network = "irc.example.invalid";
    state.channel = "";
    return luca_pbmp_serve(socket_path, &state) == 0 ? 0 : 1;
}
