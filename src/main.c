#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);

    while (1) {
        printf("$ ");

        char input[100];

        if (fgets(input, 100, stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strncmp(input, "exit", 4) == 0) {
            break;
        }

        printf("%s: command not found\n", input);
    }

    return 0;
}
