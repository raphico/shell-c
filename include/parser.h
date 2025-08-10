#pragma once

#include <stddef.h>

enum { MAX_TOKENS = 10 };

char **tokenize_input(char *input, int *argc);
