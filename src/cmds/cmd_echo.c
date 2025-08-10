#include <stdio.h>

int cmd_echo(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        printf("%s ", args[i]);
    }

    printf("\n");
    return 0;
}
