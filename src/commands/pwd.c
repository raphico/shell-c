#include "shell.h"
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>

int exec_pwd(int argc, char **argv) {
    (void)argv; // suppress warning
    (void)argc;

    char cwd[PATH_MAX];

    if (!getcwd(cwd, PATH_MAX)) {
        return -1;
    }

    printf("%s\n", cwd);
    return 0;
}
