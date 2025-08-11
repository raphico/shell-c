#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int exec_cd(cmd_ctx_t *ctx) {
    if (ctx->argc > 2) {
        fprintf(stderr, "cd: too many arguments\n");
        return -1;
    }

    if (strcmp(ctx->argv[1], "~") == 0) {
        char *home_dir = getenv("HOME");
        if (!home_dir) {
            return -1;
        }

        return chdir(home_dir);
    }

    if (chdir(ctx->argv[1]) < 0) {
        fprintf(stderr, "cd: %s: No such file or directory\n", ctx->argv[1]);
    }

    return 0;
}
