#include "shell.h"
#include <stdio.h>

int exec_echo(int argc, char **argv) {
    for (int i = 0; i < argc - 1; i++) {
        printf("%s ", argv[i]);
    }

    printf("\n");
    return 0;
}
