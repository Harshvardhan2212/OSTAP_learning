// Write a program that calls fork() and then calls some form of
// exec() to run the program /bin/ls. See if you can try all of the
// variants of exec(), including execl(), execle(), execlp(),
// execv(), execvp(), and execvP(). Why do you think there
// are so many variants of the same basic call?

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
  pid_t pid = fork();
  assert(-1 != pid);

  if (pid) {
    int status;
    wait(&status);
    if (WIFEXITED(status)) {
      int status_code = WEXITSTATUS(status);
      if (status_code) {
        printf("failed with error code = %d\n", status_code);
      } else {
        printf("child:: success\n");
      }
    }
    while (*envp)
      printf("%s\n", *(envp++));

    printf("program ended\n");
  } else {
    // NOTE: l -> stands for list
    //  e -> stands for environment
    //  p -> stands for path(use shell functionality to seach a binary)
    //  v -> vector pass a vector of arguments
    return execl("/usr/bin/ls", "ls", (char *)NULL);
  }
}
