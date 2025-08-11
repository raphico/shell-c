#pragma once

#include "shell.h"
#include <stddef.h>

int apply_redirects(cmd_ctx_t *ctx, int *save_fds);
void restore_fds(int *save_fds, size_t max_fd);
