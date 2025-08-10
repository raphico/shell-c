#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int exec_cd(int argc, char **argv) {
    if (argc > 2) {
        printf("cd: too many arguments\n");
        return -1;
    }

    if (strcmp(argv[0], "~") == 0) {
        char *home_dir = getenv("HOME");
        if (!home_dir) {
            return -1;
        }

        return chdir(home_dir);
    }

    if (chdir(argv[0]) < 0) {
        printf("cd: %s: No such file or directory\n", argv[0]);
    }

    return 0;
}
