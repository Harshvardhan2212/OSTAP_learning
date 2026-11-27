#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMCPUS 4
#define THROSHOLD 10

typedef struct __counter_t {
  int global;
  pthread_mutex_t glock;
  int local[NUMCPUS];
  pthread_mutex_t llock[NUMCPUS];
  int throshold;
} counter_t;

typedef struct {
  int i;
  counter_t *c;
} container;

void init_counter(counter_t *c, int throshold) {
  c->throshold = throshold;
  c->global = 0;
  pthread_mutex_init(&c->glock, NULL);
  int i;
  for (i = 0; i < NUMCPUS; i++) {
    pthread_mutex_init(&c->llock[i], NULL);
    c->local[i] = 0;
  }
}

void update(counter_t *c, int threadID, int amt) {
  int cpu = threadID % NUMCPUS;
  pthread_mutex_lock(&c->llock[cpu]);
  c->local[cpu] += amt;
  if (c->local[cpu] >= c->throshold) {
    pthread_mutex_lock(&c->glock);
    c->global += c->local[cpu];
    pthread_mutex_unlock(&c->glock);
    c->local[cpu] = 0;
  }
  pthread_mutex_unlock(&c->llock[cpu]);
}

int get(counter_t *c) {
  pthread_mutex_lock(&c->glock);
  int val = c->global;
  pthread_mutex_unlock(&c->glock);
  return val;
}
void *routine(void *arg) {
  container *c = (container *)arg;
  for (int i = 0; i < 1000; i++)
    update(c->c, c->i, 1);
  return c;
}

int main() {
  counter_t *counter_ptr = malloc(sizeof(counter_t));
  init_counter(counter_ptr, THROSHOLD);

  pthread_t pid[NUMCPUS];

  for (int i = 0; i < NUMCPUS; i++) {

    container *c = malloc(sizeof(container));
    *c = (container){i, counter_ptr};

    pthread_create(&pid[i], NULL, &routine, c);
  }

  for (int i = 0; i < NUMCPUS; i++) {
    container *c;
    pthread_join(pid[i], (void **)&c);
    free(c);
  }
  printf("counter value %d", get(counter_ptr));

  free(counter_ptr);

  return 0;
}
