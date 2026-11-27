#include <pthread.h>
#include <stdlib.h>

typedef struct __node_t {
  int key;
  struct __node_t *next;
} node_t;

typedef struct __queue_t {
  node_t *head;
  node_t *tail;
  pthread_mutex_t head_lock;
  pthread_mutex_t tail_lock;
} queue_t;

void init_queue(queue_t *q) {
  node_t *n = malloc(sizeof(node_t));
  n->next = NULL;
  q->head = q->tail = n;
  pthread_mutex_init(&q->head_lock, NULL);
  pthread_mutex_init(&q->tail_lock, NULL);
}

void queue_enqueue(queue_t *q, int key) {
  node_t *new = malloc(sizeof(node_t));
  new->key = key;
  pthread_mutex_lock(&q->tail_lock);
  q->tail->next = new;
  q->tail = new;
  pthread_mutex_unlock(&q->tail_lock);
}

int queue_dequeue(queue_t *q, int *value) {
  pthread_mutex_lock(&q->head_lock);
  node_t *curr = q->head;
  node_t *new_head = curr->next;
  if (!new_head) {
    pthread_mutex_unlock(&q->head_lock);
    return -1;
  }
  *value = new_head->key;
  q->head = new_head;
  pthread_mutex_unlock(&q->head_lock);
  free(curr);
}
