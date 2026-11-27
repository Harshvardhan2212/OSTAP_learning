#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int start;
  int end;
  int length;
  int *arr;
} container;

void *sum(void *arg) {
  container *c = (container *)arg;
  int *sum = malloc(sizeof(int));
  if (c->length < c->end)
    return (void *)sum;
  for (int i = c->start; i < c->end; i++)
    *sum += c->arr[i];
  free(arg);
  return (void *)sum;
}

int main() {
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  pthread_t p[10];
  int n = 10;
  int i = 0;
  int pc = 0;
  int total = 0;
  while (i < n) {
    int j = i;
    i += 3;
    container *p_cont = malloc(sizeof(container));
    *p_cont = (container){j, i, n, arr};
    pthread_create(&p[pc], NULL, &sum, p_cont);
    pc++;
  }
  while (pc--) {
    int *res;
    pthread_join(p[pc], (void *)res);
    total += *(int *)res;
  }
  printf("total : %d\n", total);
  return 0;
}
