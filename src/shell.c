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

cmd_handler_t get_cmd_handler(const char *name) {
    for (int i = 0, n = NO_BUILTIN_CMDS; i < n; i++) {
        if (strcmp(name, builtin_cmds[i].cmd_name) == 0) {
            return builtin_cmds[i].cmd_handler;
        }
    }

    return NULL;
}

int find_and_run_cmd(const char *name, char **argv) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        return -1;
    } else if (pid == 0) {
        // child process
        close(pipefd[0]);               // close unused write end
        dup2(pipefd[1], STDOUT_FILENO); // redirect the stdout to the write end
        close(pipefd[1]);               // close original write end

        execvp(name, argv);
        _exit(EXIT_FAILURE);
    } else {
        // parent process
        close(pipefd[1]); // close unused read end

        char buf[1024];
        ssize_t count = 0;
        while ((count = read(pipefd[0], buf, sizeof(buf) - 1)) > 0) {
            buf[count] = '\0';
            printf("%s", buf);
        }

        close(pipefd[0]);
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status); // Return child's exit code
        } else {
            return -1; // Abnormal termination
        }
    }
}
