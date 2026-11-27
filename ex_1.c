// Write a program that calls fork(). Before calling fork(), have the
// main process access a variable (e.g., x) and set its value to some-
// thing (e.g., 100). What value is the variable in the child process?
// What happens to the variable when both the child and parent change
// the value of x?

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[]) {
  int x = 100;
  pid_t pid = fork();
  assert(pid != -1);

  if (pid) {
    wait(NULL);
    x += 2;
    printf("parent :: program ended with x = %d\n", x);
  } else {
    x += 5;
    printf("child :: program ended with x = %d\n", x);
  }
}
