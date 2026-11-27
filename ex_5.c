// Now write a program that uses wait() to wait for the child process
// to finish in the parent. What does wait() return? What happens if
// you use wait() in the child?

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();
  if (pid) {
    int status;
    wait(&status);
    if (WIFEXITED(status)) {
      int status_code = WEXITSTATUS(status);
      if (status_code) {
        printf("some error in child = %d\n", status_code);
      } else {
        printf("success\n");
      }
    }
    printf("parent end\n");
  } else {
    printf("child start\n");
    wait(NULL);
    // NOTE: if no child process then wait do noting ->  correct -> 100 points
    printf("child end\n");
  }
}
