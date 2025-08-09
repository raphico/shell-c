#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);

  // Uncomment this block to pass the first stage
  printf("$ ");

  // Wait for user input
  char input[100];
  fgets(input, 100, stdin);

  input[strcspn(input, "\n")] = '\0';
  printf("%s: command not found\n", input);
  return 0;
}
