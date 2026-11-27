// Write another program using fork().The child process should print “hello”;
// the parent process should print “goodbye”.You should try to ensure that the
//     child process always prints first;
// can you do this without calling wait() in the parent ?

#include <stdio.h>
#include <unistd.h>

int main() {

  pid_t pid = fork();

  if (pid) {
    sleep(1);
    printf("goodbye\n");
  } else {
    printf("hello ");
  }
}
