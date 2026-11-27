#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_OF_THREAD 6

typedef struct __node_t {
  int key;
  struct __node_t *next;
} node_t;

typedef struct {
  node_t *head;
  pthread_mutex_t lock;
} list_t;

void list_init(list_t *l) {
  l->head = NULL;
  pthread_mutex_init(&l->lock, NULL);
}

int list_insert(list_t *l, int key) {
  pthread_mutex_lock(&l->lock);
  node_t *new = malloc(sizeof(node_t));
  if (!new) {
    perror("malloc");
    pthread_mutex_unlock(&l->lock);
    return -1;
  }
  new->key = key;
  new->next = l->head;
  l->head = new;
  pthread_mutex_unlock(&l->lock);
  return 0;
}

int list_lookup(list_t *l, int key) {
  pthread_mutex_lock(&l->lock);
  node_t *curr = l->head;
  while (curr) {
    if (curr->key == key)
      return 1;

    curr = curr->next;
  }
  pthread_mutex_unlock(&l->lock);
  return -1;
}

void print_list(list_t *l) {
  node_t *curr = l->head;
  int i = 0;
  while (curr) {
    printf("%d : [ %d]\n", i, curr->key);
    curr = curr->next;
    i++;
  }
}

void *routine(void *arg) {
  list_t *l = (list_t *)arg;
  for (int i = 0; i < rand() % 20; i++) {
    list_insert(l, rand());
  }
  return NULL;
}

int main() {
  pthread_t pid[NUM_OF_THREAD];
  srand(time(NULL));
  list_t *l = malloc(sizeof(list_t));
  list_init(l);

  for (int i = 0; i < NUM_OF_THREAD; i++) {
    pthread_create(&pid[i], NULL, &routine, l);
  }

  for (int i = 0; i < NUM_OF_THREAD; i++) {
    pthread_join(pid[i], NULL);
  }

  print_list(l);
  pthread_mutex_destroy(&l->lock);
  free(l);
  return 0;
}
