#include "cmd.h"
#include "utils.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

        char *path = getenv("PATH");
        if (!path) {
            continue;
        }

        char *path_copy = strdup(path);
        if (!path_copy) {
            continue;
        }

        char *token = strtok(path_copy, ":");
        while (token != NULL) {
            DIR *dir = opendir(token);
            if (!dir) {
                token = strtok(NULL, ":");
                continue;
            }

            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL) {
                if (strcmp(args[i], entry->d_name) == 0) {
                    found = true;
                    printf("%s is %s/%s\n", args[i], token, entry->d_name);
                    break;
                }
            }
            closedir(dir);

            if (found) {
                break;
            }

            token = strtok(NULL, ":");
        }

        free(path_copy);

        if (!found) {
            printf("%s: not found\n", args[i]);
        }
    }

    return 0;
}