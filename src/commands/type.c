#include "shell.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int exec_type(int argc, char **argv) {
    for (int i = 0; i < argc - 1; i++) {
        bool found = false;

        for (int j = 0, n = NO_BUILTIN_CMDS; j < n; j++) {
            if (strcmp(argv[i], builtin_cmds[j].cmd_name) == 0) {
                printf("%s is a shell builtin\n", argv[i]);
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
                if (strcmp(argv[i], entry->d_name) == 0) {
                    char full_path[PATH_MAX];
                    snprintf(full_path, PATH_MAX, "%s/%s", token,
                             entry->d_name);

                    if (access(full_path, X_OK) == 0) {
                        found = true;
                        printf("%s is %s\n", argv[i], full_path);
                        break;
                    }
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
            printf("%s: not found\n", argv[i]);
        }
    }

    return 0;
}
