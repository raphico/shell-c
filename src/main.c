#include "cmd.h"
#include "parser.h"
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
        char **args = tokenize_input(input);
        if (!args || !args[0]) {
            free(args);
            continue;
        }

        cmd_handler_t handler = get_cmd_handler(args[0]);
        if (!handler) {
            if (find_and_run_cmd(args[0], args + 1) != 0) {
                printf("%s: command not found\n", args[0]);
            }
        } else {
            int status = handler(args + 1);
            if (status == SHELL_EXIT) {
                return 0;
            }
        }

        free(args);
    }

    return 0;
}
