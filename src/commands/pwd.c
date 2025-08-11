#include "shell.h"
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>

int exec_pwd(cmd_ctx_t *ctx) {
    (void)ctx; // suppress warning

    char cwd[PATH_MAX];

    if (!getcwd(cwd, PATH_MAX)) {
        return -1;
    }

    printf("%s\n", cwd);
    return 0;
}
