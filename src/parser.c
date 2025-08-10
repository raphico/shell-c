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
    bool in_squote = false, in_dquote = false, is_escaped = false;
    char buf[1024];

    while (i < n) {
        char c = input[i];

        if (c == '\\' && !is_escaped && !in_squote) {
            i++;
            is_escaped = true;
            continue;
        }

        if (c == '\'' && !in_dquote && !is_escaped) {
            in_squote = !in_squote;
            i++;
            continue;
        }

        if (c == '"' && !in_squote && !is_escaped) {
            in_dquote = !in_dquote;
            i++;
            continue;
        }

        if (c == ' ' && !in_squote && !in_dquote) {
            if (j > 0 || is_escaped) {
                buf[j] = '\0';
                tokens[count++] = strdup(buf);
                j = 0;
            }

            is_escaped = false;
            i++;
            continue;
        }

        if (j < (int)sizeof(buf) - 1) {
            buf[j++] = c;
        }

        i++;
        is_escaped = false;
    }

    if (j > 0) {
        buf[j] = '\0';
        tokens[count++] = strdup(buf);
    }

    tokens[count] = NULL;

    *argc = count;
    return tokens;
}
