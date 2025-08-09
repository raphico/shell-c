#include "cmd.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

cmd_t builtin_cmds[] = {
    {.name = "exit", .handler = cmd_exit},
    {.name = "echo", .handler = cmd_echo},
    {.name = "type", .handler = cmd_type},
};

cmd_handler_t get_cmd_handler(const char *name) {
    for (int i = 0, n = ARRAY_SIZE(builtin_cmds); i < n; i++) {
        if (strcmp(name, builtin_cmds[i].name) == 0) {
            return builtin_cmds[i].handler;
        }
    }

    return NULL;
}

int cmd_echo(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        printf("%s ", args[i]);
    }

    printf("\n");
    return 0;
}

int cmd_exit(char **args) {
    (void)args; // suppress warnings
    return SHELL_EXIT;
}

int cmd_type(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        bool found = false;
        for (int j = 0, n = ARRAY_SIZE(builtin_cmds); j < n; j++) {
            if (strcmp(args[i], builtin_cmds[j].name) == 0) {
                printf("%s is a shell builtin\n", args[i]);
                found = true;
                break;
            }
        }

        if (!found) {
            printf("%s not found\n", args[i]);
        }
    }

    return 0;
}