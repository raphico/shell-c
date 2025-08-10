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
    bool in_squote = false, in_dquote = false;
    char buf[1024];

    while (i < n) {
        char c = input[i];

        if (c == '\'' && !in_dquote) {
            in_squote = !in_squote;
            i++;
            continue;
        }

        if (c == '"' && !in_squote) {
            in_dquote = !in_dquote;
            i++;
            continue;
        }

        // handles backslash
        if (c == '\\' && !in_squote) {
            /* If we're in "" quotes  only the following chars are escaped: ",
             \, $, `, \n otherwise(unquoted), backslash escapes the next char */
            if (in_dquote) {
                if (i + 1 < n) {
                    char next = input[i + 1];
                    if (next == '"' || next == '\\' || next == '$' ||
                        next == '`' || next == '\n') {
                        if (j < (int)sizeof(buf) - 1) {
                            buf[j++] = next;
                        }

                        i += 2;
                        continue;
                    } else {
                        // interpret the backslash here literally
                        if (j < (int)sizeof(buf) - 1) {
                            buf[j++] = '\\';
                        }
                        i++;
                        continue;
                    }
                } else {
                    // trailing backslash here, interpret literally
                    if (j < (int)sizeof(buf) - 1) {
                        buf[j++] = '\\';
                    }

                    i++;
                    continue;
                }
            } else {
                // not in a quote: backslash escapes the next character if
                // present
                if (i + 1 < n) {
                    char next = input[i + 1];
                    if (j < (int)sizeof(buf) - 1) {
                        buf[j++] = next;
                    }

                    i += 2;
                    continue;
                } else {
                    // trailing backslash here, interpret literally
                    if (j < (int)sizeof(buf) - 1) {
                        buf[j++] = '\\';
                    }

                    i++;
                    continue;
                }
            }
        }

        // split token on unquoted space
        if (c == ' ' && !in_dquote && !in_squote) {
            if (j > 0) {
                buf[j] = '\0';
                if (count < MAX_TOKENS) {
                    tokens[count++] = strdup(buf);
                }

                j = 0;
            }

            i++;
            continue;
        }

        // append if regular character
        if (j < (int)sizeof(buf) - 1) {
            buf[j++] = c;
        }

        i++;
    }

    if (j > 0) {
        buf[j] = '\0';
        if (count < MAX_TOKENS) {
            tokens[count++] = strdup(buf);
        }
    }

    tokens[count] = NULL;
    *argc = count;
    return tokens;
}
