#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "list_struct.h"

#define INF 1000000

void *test_iterating(void *arg) {
  for(int i =0;i<INF;i++)
  {
    print_data();
    sleep(5);
  }
}

void *test_add_and_remove(void *arg) {
  for (int i = 1; i < INF; i++) {
    insert(i, 2 * i);
    remove_node(i);
  }
}
void *test_add_then_remove(void *arg) {
  for (int i = 1; i < INF; i++) {
    insert(i, 2 * i);
  }

  for (int i = 1; i < INF; i++) {
    remove_node(i);
  }
}

void *test_get(void *arg) {
  srand(time(NULL));
  int r;
  for (int i = 0; i < INF; i++) {
    r = rand() % 10 + 1;
    get(r);
  }
}

void *test_poll(void *arg) {
  srand(time(NULL));
  int r;
  for (int i = 0; i < INF; i++) {
    r = rand() % 10 + 1;
    poll(r);
  }
}

int main() {

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  pthread_t thread[5];
  int temp;
  temp = pthread_create(&thread[0], NULL, test_iterating, NULL);
  if (temp != 0) {
    printf("can not open thread 0");
  }
  temp = pthread_create(&thread[1], NULL, test_add_and_remove, NULL);
  if (temp != 0) {
    printf("can not open thread 1");
  }
  temp = pthread_create(&thread[2], NULL, test_add_then_remove, NULL);
  if (temp != 0) {
    printf("can not open thread 2");
  }
  temp = pthread_create(&thread[3], NULL, test_get, NULL);
  if (temp != 0) {
    printf("can not open thread 3");
  }
  temp = pthread_create(&thread[4], NULL, test_poll, NULL);
  if (temp != 0) {
    printf("can not open thread 4");
  }

  for (int i = 0; i < 5; i++) {
    pthread_join(thread[i], NULL);
  }

  return 0;
}
