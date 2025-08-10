#include "shell.h"

int exec_exit(int argc, char **argv) {
    (void)argv; // suppress warnings
    (void)argc;

    return SHELL_EXIT;
}
