#pragma once

#include <stdbool.h>

enum { SHELL_EXIT = -2, NO_BUILTIN_CMDS = 5 };

typedef struct redirect_t {
    int fd;
    char *filename;
    bool overwrite;
    struct redirect_t *next;
} redirect_t;

typedef struct {
    int argc;
    char **argv;
    redirect_t *redir;
} cmd_ctx_t;

typedef int (*cmd_handler_t)(cmd_ctx_t *ctx);
void cleanup_cmd_ctx(cmd_ctx_t *ctx);

typedef struct {
    const char *cmd_name;
    cmd_handler_t cmd_handler;
} shell_cmd_t;

int exec_exit(cmd_ctx_t *ctx);
int exec_echo(cmd_ctx_t *ctx);
int exec_type(cmd_ctx_t *ctx);
int exec_pwd(cmd_ctx_t *ctx);
int exec_cd(cmd_ctx_t *ctx);

extern shell_cmd_t builtin_cmds[NO_BUILTIN_CMDS];

cmd_handler_t get_cmd_handler(const char *name);
int find_and_run_cmd(const char *name, char **argv);
