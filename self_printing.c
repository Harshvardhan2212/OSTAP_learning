#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[]) {
  pid_t pid = fork();
  assert(pid != -1);

  if (pid) {
    wait(NULL);
    printf("program ended\n");
  } else {
    char *arg_exc[2];
    arg_exc[0] = "cat";
    arg_exc[1] = "self_printing.c";
    execvp(arg_exc[0], arg_exc);
  }
}
