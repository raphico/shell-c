#include "shell.h"

int exec_exit(cmd_ctx_t *ctx) {
    (void)ctx; // suppress warnings

    return SHELL_EXIT;
}
