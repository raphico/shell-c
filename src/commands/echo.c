#include "shell.h"
#include <stdio.h>

int exec_echo(cmd_ctx_t *ctx) {
    for (int i = 1; i < ctx->argc; i++) {
        printf("%s ", ctx->argv[i]);
    }

    printf("\n");
    return 0;
}
