#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define CONS_IT 5
#define NUM_OF_THREAD 4
#define MAX_BUFF_SIZE 100

pthread_cond_t fill, empty;
pthread_mutex_t mutex;
int buffer[MAX_BUFF_SIZE];
int count = 0;
int fill_ptr = 0;
int empty_ptr = 0;

typedef struct __container {
  int tnum;
} container;

void put(int value) {
  buffer[fill_ptr] = value;
  fill_ptr = (fill_ptr + 1) % MAX_BUFF_SIZE;
  count++;
}

int get() {
  int val = buffer[empty_ptr];
  empty_ptr = (empty_ptr + 1) % MAX_BUFF_SIZE;
  count--;
  return val;
}

void *provider(void *arg) {
  container *c = (container *)arg;
  for (int i = 0; i < CONS_IT * (NUM_OF_THREAD - 1); i++) {
    pthread_mutex_lock(&mutex);
    while (count == MAX_BUFF_SIZE)
      pthread_cond_wait(&empty, &mutex);
    printf("putting value in buffer from thread %d => %d\n", c->tnum, i);
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

    printf("got from producer value from thread %d => %d\n", c->tnum, get());
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);
  }
  return c;
}

int main() {
  pthread_t pid[NUM_OF_THREAD];
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&fill, NULL);
  pthread_cond_init(&empty, NULL);
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

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&fill);
  pthread_cond_destroy(&empty);
  return 0;
}
