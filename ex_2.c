// Write a program that opens a file (with the open() system call)
// and then calls fork() to create a new process. Can both the child
// and parent access the file descriptor returned by open()? What
// happens when they are writing to the file concurrently, i.e., at the
// same time?

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[]) {
  int fd = open("ex_2.txt", O_WRONLY | O_CREAT, 0777);
  pid_t pid = fork();
  assert(pid != -1);

  if (pid) {
    char parent_txt[] = "hello this is from a parent\n";
    if (-1 == write(fd, parent_txt, sizeof(parent_txt))) {
      printf("parent :: write error with errno = %d", errno);
      return 1;
    }
    close(fd);
  } else {
    char child_txt[] = "hello this is from a child\n";
    if (-1 == write(fd, child_txt, sizeof(child_txt))) {
      printf("child :: write error with errno = %d", errno);
      return 1;
    }
    close(fd);
  }
}
