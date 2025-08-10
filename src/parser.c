#include "parser.h"
#include <stdlib.h>
#include <string.h>

char **tokenize_input(char *input, int *argc) {
    // + 1 for a NULL pointer that marks the end of the tokens
    char **tokens = malloc(sizeof(char *) * (MAX_TOKENS + 1));
    if (!tokens) {
        return NULL;
    }

    char *token = strtok(input, " \t\n");

    int i = 0;
    while (token != NULL && i < MAX_TOKENS) {
        tokens[i++] = token;
        token = strtok(NULL, " \t\n");
    }

    tokens[i] = NULL;

    *argc = i;

    return tokens;
}
