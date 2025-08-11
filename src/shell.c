#include "shell.h"
#include <dirent.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

shell_cmd_t builtin_cmds[] = {
    {.cmd_name = "exit", .cmd_handler = exec_exit},
    {.cmd_name = "echo", .cmd_handler = exec_echo},
    {.cmd_name = "type", .cmd_handler = exec_type},
    {.cmd_name = "pwd", .cmd_handler = exec_pwd},
    {.cmd_name = "cd", .cmd_handler = exec_cd},
};

void cleanup_cmd_ctx(cmd_ctx_t *ctx) {
    if (!ctx)
        return;

    for (int i = 0; i < ctx->argc; i++) {
        free(ctx->argv[i]);
    }
    free(ctx->argv);

    redirect_t *ptr = ctx->redir;
    while (ptr != NULL) {
        redirect_t *next = ptr->next;
        free(ptr->filename);
        free(ptr);
        ptr = next;
    }
    free(ctx);
}

cmd_handler_t get_cmd_handler(const char *name) {
    for (int i = 0, n = NO_BUILTIN_CMDS; i < n; i++) {
        if (strcmp(name, builtin_cmds[i].cmd_name) == 0) {
            return builtin_cmds[i].cmd_handler;
        }
    }

    return NULL;
}

int find_and_run_cmd(const char *name, char **argv) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        // child: execute command
        execvp(name, argv);
        perror(name);
        _exit(127); // special code: command not found
    }

    // parent: blocks until the child finishes and returns its exit code
    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return -1;
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    return -1;
}
