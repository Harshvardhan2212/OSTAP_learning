#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define CONS_IT 5
#define NUM_OF_THREAD 4

pthread_cond_t fill, empty;
pthread_mutex_t mutex;
int buffer;
int count = 0;

typedef struct __container {
  int tnum;
} container;

void put(int value) {
  assert(count == 0);
  buffer = value;
  count = 1;
}

int get() {
  assert(count == 1);
  count = 0;
  return buffer;
}

void *provider(void *arg) {
  container *c = (container *)arg;
  for (int i = 0; i < CONS_IT * (NUM_OF_THREAD - 1); i++) {
    pthread_mutex_lock(&mutex);
    while (count == 1)
      pthread_cond_wait(&empty, &mutex);
    printf("putting %d in buffer from thread %d\n", i, c->tnum);
    put(i);
    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&mutex);
  }
  return c;
}

void *consumer(void *arg) {
  container *c = (container *)arg;
  for (int i = 0; i < CONS_IT; i++) {
    pthread_mutex_lock(&mutex);
    while (count == 0)
      pthread_cond_wait(&fill, &mutex);

    printf("got from producer: %d from thread %d\n", get(), c->tnum);
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);
  }
  return c;
}

int main() {
  pthread_t pid[NUM_OF_THREAD];

  for (int i = 0; i < 4; i++) {
    container *c = malloc(sizeof(container));
    *c = (container){i};
    if (i == 2)
      pthread_create(&pid[i], NULL, &provider, c);
    else
      pthread_create(&pid[i], NULL, &consumer, c);
  }

  for (int i = 0; i < 4; i++) {
    container *c;
    pthread_join(pid[i], (void **)&c);
    free(c);
  }
  return 0;
}
