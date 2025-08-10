#pragma once

enum { SHELL_EXIT = -2, NO_BUILTIN_CMDS = 4 };

typedef int (*cmd_handler_t)(char **args);

typedef struct {
    const char *name;
    cmd_handler_t handler;
} cmd_t;

int cmd_exit(char **args);
int cmd_echo(char **args);
int cmd_type(char **args);
int cmd_pwd(char **args);

extern cmd_t builtin_cmds[NO_BUILTIN_CMDS];

cmd_handler_t get_cmd_handler(const char *name);
int find_and_run_cmd(const char *name, char **args);
