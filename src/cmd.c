#include "cmd.h"
#include <dirent.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

cmd_t builtin_cmds[] = {
    {.name = "exit", .handler = cmd_exit},
    {.name = "echo", .handler = cmd_echo},
    {.name = "type", .handler = cmd_type},
};

cmd_handler_t get_cmd_handler(const char *name) {
    for (int i = 0, n = NO_BUILTIN_CMDS; i < n; i++) {
        if (strcmp(name, builtin_cmds[i].name) == 0) {
            return builtin_cmds[i].handler;
        }
    }

    return NULL;
}

int find_and_run_cmd(const char *name, char **args) {
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();
    if (pid < 0) {
        return -1;
    } else if (pid == 0) {
        // child process
        close(pipefd[0]);               // close unused write end
        dup2(pipefd[1], STDOUT_FILENO); // redirect the stdout to the write end
        close(pipefd[1]);               // close original write end

        execvp(name, args);
        return -1;
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
        waitpid(pid, NULL, 0);

        return 0;
    }
}
