#include "cmd.h"

int cmd_exit(char **args) {
    (void)args; // suppress warnings
    return SHELL_EXIT;
}
