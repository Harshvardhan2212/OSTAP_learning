// Write a program that creates two children, and connects the stan-
// dard output of one to the standard input of the other, using the
// pipe() system call.

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int fd[2];
  assert(-1 != pipe(fd));
  pid_t pid = fork();
  assert(-1 != pid);

  if (pid) {
    pid_t new_child = fork();
    if (new_child == 0) {
      printf("child 2 initiated\n");

      close(fd[1]);

      int read_size;
      assert(-1 != read(fd[0], &read_size, sizeof(int)));

      char message_reciver[read_size];
      assert(-1 != read(fd[0], &message_reciver, read_size));
      message_reciver[read_size] = '\0';
      close(fd[0]);

      printf("%s\n", message_reciver);
    } else {
      wait(NULL);
    }
  } else {
    printf("child 1 initiated\n");

    close(fd[0]);

    char message[] =
        "this is a message from a first child to second you are adopted";

    int length = strlen(message);

    assert(-1 != write(fd[1], &length, sizeof(int)));

    assert(-1 != write(fd[1], &message, sizeof(message)));
    close(fd[1]);
  }
}
