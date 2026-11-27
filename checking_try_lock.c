#include <assert.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t fuel_mutex;
pthread_cond_t fuel_cond;
size_t fuel = 0;

void *fill_fuel(void *arg) {
  for (int i = 0; i < 5; i++) {

    pthread_mutex_lock(&fuel_mutex);
    fuel += 15;
    printf("filling pump...\n");
    printf("fuel is now %zu...\n", fuel);
    pthread_mutex_unlock(&fuel_mutex);
    pthread_cond_signal(&fuel_cond);
  }
  return NULL;
}

void *car_filling(void *arg) {
  pthread_mutex_lock(&fuel_mutex);
  while (fuel < 40) {
    printf("not enough fuel...");
    pthread_cond_wait(&fuel_cond, &fuel_mutex);
  }

  fuel -= 40;

  printf("remaining fuel %zu...\n", fuel);
  pthread_mutex_unlock(&fuel_mutex);
  printf("car filled...\n");
  return NULL;
}

int main() {
  pthread_mutex_init(&fuel_mutex, NULL);
  pthread_cond_init(&fuel_cond, NULL);

  pthread_t pid[10];
  for (int i = 0; i < 6; i++) {
    if (i % 2) {
      pthread_create(&pid[i], NULL, &car_filling, NULL);
    } else
      pthread_create(&pid[i], NULL, &fill_fuel, NULL);
  }

  for (int i = 0; i < 6; i++)
    pthread_join(pid[i], NULL);
  pthread_mutex_destroy(&fuel_mutex);
  pthread_cond_destroy(&fuel_cond);
}
