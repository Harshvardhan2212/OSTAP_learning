// Write a slight modification of the previous program, this time us-
// ing waitpid() instead of wait(). When would waitpid() be
// useful?

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();
  if (pid) {
    int status;
    waitpid(0, &status, WUNTRACED);
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
