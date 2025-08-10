#include "cmd.h"
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>

int cmd_pwd(char **args) {
    (void)args; // suppress warning

    char cwd[PATH_MAX];

    if (!getcwd(cwd, PATH_MAX)) {
        return -1;
    }

    printf("%s\n", cwd);
    return 0;
}
