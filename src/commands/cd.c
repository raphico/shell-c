#include "shell.h"
#include <stdio.h>
#include <unistd.h>

int exec_cd(int argc, char **argv) {
    if (argc > 2) {
        printf("cd: too many arguments\n");
        return -1;
    }

    if (chdir(argv[0]) < 0) {
        printf("cd: %s: No such file or directory\n", argv[0]);
    }

    return 0;
}
