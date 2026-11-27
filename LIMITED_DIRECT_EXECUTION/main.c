#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

long long int print_time_diff() {
  struct timespec start, end;

  int fd = open("file.txt", O_RDWR | O_CREAT, 0777);
  if (write(fd, "hello", 5) == -1) {
    printf("errno : %d\n", errno);
  }
  printf("old fd =  %d\n", fd);
  close(fd);

  int fd_new = open("file.txt", O_RDONLY, 0777);
  printf("old fd =  %d\n", fd_new);
  char mes[5];
  clock_gettime(CLOCK_MONOTONIC, &start);

  if (read(fd, &mes, 5) == -1) {
    printf("errno : %d\n", errno);
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  printf("message: %s\n", mes);

  long long elapsed_ns = ((long long)end.tv_sec - start.tv_sec) * 1000000000LL +
                         (end.tv_nsec - start.tv_nsec);

  printf("Elapsed: %lld ns\n", elapsed_ns);

  return elapsed_ns;
}

// int *processing_time() {
//   int *res = malloc(sizeof(int) * 4);
//   struct timeval t_val;
//   struct timeval t_val_after;
//   struct timezone tz;
//   gettimeofday(&t_val, &tz);
//   int fd = open("file.txt", O_RDONLY | O_CREAT, 0777);
//   gettimeofday(&t_val_after, &tz);
//   printf("open diff sec: %lld and diff micro_sec: %lld\n",
//          (long long)t_val_after.tv_sec - t_val.tv_sec,
//          (long long)t_val_after.tv_usec - t_val.tv_usec);
//
//   gettimeofday(&t_val, &tz);
//   write(fd, "hello", 5);
//   gettimeofday(&t_val_after, &tz);
//   printf("write diff sec: %lld and diff micro_sec: %lld\n",
//          (long long)t_val_after.tv_sec - t_val.tv_sec,
//          (long long)t_val_after.tv_usec - t_val.tv_usec);
//
//   close(fd);
//   return res;
// }

int main() {
  long long int res[10];
  int i = 0;
  long long int sum = 0;
  while (i < 10) {
    res[i] = print_time_diff();
    sum += res[i];
    i++;
  }
  printf("avg time = %lld ns\n", sum / 10);
  return 0;
}
