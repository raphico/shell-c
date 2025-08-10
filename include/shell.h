#pragma once

enum { SHELL_EXIT = -2, NO_BUILTIN_CMDS = 5 };

typedef int (*cmd_handler_t)(int argc, char **argv);

typedef struct {
    const char *cmd_name;
    cmd_handler_t cmd_handler;
} shell_cmd_t;

int exec_exit(int argc, char **argv);
int exec_echo(int argc, char **argv);
int exec_type(int argc, char **argv);
int exec_pwd(int argc, char **argv);
int exec_cd(int argc, char **argv);

extern shell_cmd_t builtin_cmds[NO_BUILTIN_CMDS];

cmd_handler_t get_cmd_handler(const char *name);
int find_and_run_cmd(const char *name, char **argv);
