#pragma once

enum { SHELL_EXIT = -1 };

typedef int (*cmd_handler_t)(char **args);

typedef struct {
    const char *name;
    cmd_handler_t handler;
} cmd_t;

int cmd_exit(char **args);
int cmd_echo(char **args);
int cmd_type(char **args);

cmd_handler_t get_cmd_handler(const char *name);
