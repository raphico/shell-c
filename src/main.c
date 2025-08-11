#include "parser.h"
#include "redirects.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    while (1) {
        printf("$ ");

        char input[100];

        if (fgets(input, 100, stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        int tokc;
        char **tokens = tokenize_input(input, &tokc);
        if (!tokens || tokc == 0) {
            // nothing to run or tokenization failed
            if (tokens) {
                for (int i = 0; i < tokc; i++) {
                    free(tokens[i]);
                }
                free(tokens);
            }
            continue;
        }

        cmd_ctx_t *ctx = parse_redirects(tokc, tokens);
        if (!ctx) {
            for (int i = 0; i < tokc; i++) {
                free(tokens[i]);
            }
            free(tokens);
            continue;
        }

        // STDIN (0), STDOUT (1), and STDERR (2)
        int save_fds[3] = {-1, -1, -1};

        if (apply_redirects(ctx, save_fds) == 0) {
            cmd_handler_t handler = get_cmd_handler(ctx->argv[0]);
            if (!handler) {
                if (find_and_run_cmd(ctx->argv[0], ctx->argv) != 0) {
                    fprintf(stderr, "%s: command not found\n", argv[0]);
                }
            } else {
                int status = handler(ctx);
                if (status == SHELL_EXIT) {
                    cleanup_cmd_ctx(ctx);
                    for (int i = 0; i < argc; i++) {
                        free(tokens[i]);
                    }
                    free(tokens);
                    break;
                }
            }

            restore_fds(save_fds, 3);
        }

        for (int i = 0; i < argc; i++) {
            free(tokens[i]);
        }
        free(tokens);
        cleanup_cmd_ctx(ctx);
    }

    return 0;
}
