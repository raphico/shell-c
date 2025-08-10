#include "parser.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char **tokenize_input(char *input, int *argc) {
    char **tokens = malloc(sizeof(char *) * (MAX_TOKENS + 1));
    if (!tokens) {
        return NULL;
    }

    int i = 0, j = 0, count = 0, n = strlen(input);
    bool in_quote = false;
    char buf[1024];

    while (i < n) {
        char c = input[i];

        if (c == '\'') {
            in_quote = !in_quote;
            i++;
            continue;
        }

        if (c == ' ' && !in_quote) {
            if (j > 0) {
                buf[j] = '\0';
                tokens[count++] = strdup(buf);
                j = 0;
            }

            i++;
            continue;
        }


        if (j < (int)sizeof(buf)) {
            buf[j++] = c;
        }

        i++;
    }

    if (j > 0) {
        buf[j] = '\0';
        tokens[count++] = strdup(buf);
    }

    tokens[count] = NULL;

    *argc = count;
    return tokens;
}