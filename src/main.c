#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);

  while (1) {
    printf("$ ");
    char input[100];
    fgets(input, 100, stdin);

    input[strcspn(input, "\n")] = '\0';
    printf("%s: command not found\n", input);
  }

  return 0;
}
