#include "parser.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);

    while (1) {
        printf("$ ");

        char input[100];

        if (fgets(input, 100, stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        int argc;
        char **argv = tokenize_input(input, &argc);
        if (!argv || !argv[0]) {
            free(argv);
            continue;
        }

        cmd_handler_t handler = get_cmd_handler(argv[0]);
        if (!handler) {
            if (find_and_run_cmd(argv[0], argv) != 0) {
                printf("%s: command not found\n", argv[0]);
            }
        } else {
            int status = handler(argc, argv + 1);
            if (status == SHELL_EXIT) {
                return 0;
            }
        }

        for (int i = 0; i < argc; i++) {
            free(argv[i]);
        }

        free(argv);
    }

    return 0;
}
