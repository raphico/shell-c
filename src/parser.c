#include "parser.h"
#include "shell.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static redirect_t *alloc_and_link_redirect(cmd_ctx_t *cmd_ctx) {
    redirect_t *new_redir = calloc(1, sizeof(redirect_t));
    if (!new_redir) {
        return NULL;
    }

    if (!cmd_ctx->redir) {
        cmd_ctx->redir = new_redir;
    } else {
        redirect_t *ptr = cmd_ctx->redir;
        while (ptr->next) {
            ptr = ptr->next;
        }
        ptr->next = new_redir;
    }

    return new_redir;
}

int parse_fd_prefix(const char **ptok) {
    const char *tok = *ptok;

    if (isdigit((unsigned char)tok[0])) {
        char *end;
        long val = strtol(tok, &end, 10);
        if (*end == '>') { // protect against weird cases like 123abc
            *ptok = end;
            return (int)val;
        }
    }

    return 1;
}

cmd_ctx_t *parse_redirects(int argc, char **tokens) {
    cmd_ctx_t *cmd_ctx = calloc(1, sizeof(cmd_ctx_t));
    if (!cmd_ctx) {
        return NULL;
    }

    cmd_ctx->argv = malloc(sizeof(char *) * (argc + 1));
    if (!cmd_ctx->argv) {
        free(cmd_ctx);
        return NULL;
    }

    for (int i = 0; i < argc; i++) {
        char *tok = tokens[i];
        const char *p = tok;
        int fd = parse_fd_prefix(&p);

        bool is_append = false;
        size_t len = strlen(p);

        if (len > 0 && p[0] == '>') {
            if (len >= 2 && p[1] == '>') {
                is_append = true;
                p += 2;
            } else {
                p++;
            }

            char *filename = NULL;
            if (*p != '\0') {
                if (*p == '>') {
                    fprintf(stderr, "sh: parse error `>`\n");
                    cleanup_cmd_ctx(cmd_ctx);
                    return NULL;
                }
                filename = strdup(p);
            } else {
                if (i + 1 >= argc) {
                    fprintf(stderr, "sh: parse error near `\n'");
                    cleanup_cmd_ctx(cmd_ctx);
                    return NULL;
                }

                filename = strdup(tokens[++i]);
            }

            redirect_t *new_redir = alloc_and_link_redirect(cmd_ctx);
            if (!new_redir) {
                free(filename);
                cleanup_cmd_ctx(cmd_ctx);
                return NULL;
            }

            new_redir->fd = fd;
            new_redir->filename = filename;
            new_redir->overwrite = !is_append;
        } else {
            cmd_ctx->argv[cmd_ctx->argc++] = strdup(tok);
            if (!cmd_ctx->argv[cmd_ctx->argc - 1]) {
                cleanup_cmd_ctx(cmd_ctx);
                return NULL;
            }
        }
    }

    cmd_ctx->argv[cmd_ctx->argc] = NULL;
    return cmd_ctx;
}

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
                /* not in a quote: backslash escapes the next character if
                 present */
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
