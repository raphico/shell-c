#include "cmd.h"
#include <stdio.h>
#include <unistd.h>

int cmd_cd(char **args) {
    int argc = 0;
    while (args[argc] != NULL) {
        argc++;
    }

    if (argc > 1) {
        printf("cd: too many arguments\n");
        return -1;
    }

    if (chdir(args[0]) < 0) {
        printf("cd: %s: No such file or directory\n", args[0]);
    }

    return 0;
}
