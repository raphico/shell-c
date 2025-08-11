#pragma once

#include "shell.h"
#include <stddef.h>

enum { MAX_TOKENS = 10 };

char **tokenize_input(char *input, int *argc);
int test_redirop(const char *pattern, const char *input, cmd_ctx_t *cmd_ctx);
cmd_ctx_t *parse_redirects(int argc, char **tokens);
