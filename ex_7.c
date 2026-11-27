// Write a program that creates a child process, and then in the child
// closes standard output (STDOUT FILENO). What happens if the child
// calls printf() to print some output after closing the descriptor?
//
// -> write noting in statndard output

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();
  if (pid) {
    wait(NULL);
    printf("parent end\n");
  } else {
    close(STDOUT_FILENO);
    printf("printing this from child process\n");
  }
}
