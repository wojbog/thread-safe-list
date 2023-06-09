#include "list_struct.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct node *head = NULL;

pthread_mutex_t mutex;
pthread_cond_t cond;

struct node *first() { return head; }

struct node *next(struct node *obj) {
  pthread_mutex_lock(&mutex);
  if (obj == NULL) {
    pthread_mutex_unlock(&mutex);
    return NULL;
  }
  pthread_mutex_unlock(&mutex);
  return obj->next;
}

struct node *last() {
  pthread_mutex_lock(&mutex);
  struct node *temp = head;
  if (head == NULL) {
    pthread_mutex_unlock(&mutex);
    return head;
  }

  while (temp->next != NULL) {
    temp = temp->next;
  }
  pthread_mutex_unlock(&mutex);
  return temp;
}

void insert(int key, int value) {
  struct node *new_node = malloc(sizeof(struct node));

  new_node->key = key;
  new_node->value = value;
  new_node->next = NULL;

  pthread_mutex_lock(&mutex);

  struct node *temp = head;
  if (head == NULL) {
    head = new_node;
    printf("added succesfully key: %d, value: %d\n", key, value);
    pthread_mutex_unlock(&mutex);
    return;
  }

  while (temp->next != NULL && temp->key != key) {
    temp = temp->next;
  }

  if (temp->key == key) {
    printf("key %d already in the list\n", key);
    pthread_mutex_unlock(&mutex);
    return;
  }

  temp->next = new_node;
  printf("added succesfully key: %d, value: %d\n", key, value);
  pthread_cond_broadcast(&cond);
  pthread_mutex_unlock(&mutex);
}

int get(int key) {
  pthread_mutex_lock(&mutex);

  struct node *temp = head;
  if (temp == NULL) {
    printf("There is no key: %d in the list\n", key);
    pthread_mutex_unlock(&mutex);

    return -1;
  }

  while (temp->next != NULL && temp->key != key) {
    temp = temp->next;
  }

  if (temp->key == key) {
    printf("got succesfully key: %d, value: %d\n", key, temp->value);

    pthread_mutex_unlock(&mutex);

    return temp->value;
  }

  printf("There is no key: %d in the list\n", key);
  pthread_mutex_unlock(&mutex);
  return -1;
}

int poll(int key) {
  pthread_mutex_lock(&mutex);

  struct node *temp = head;
  if (temp == NULL) {
    printf("There is no key: %d in the list\n", key);
    pthread_mutex_unlock(&mutex);
    return -1;
  }
  int status = 0;
  while (status == 0) {

    while (temp->next != NULL && temp->key != key) {
      temp = temp->next;
    }

    if (temp->key == key) {
      status = 1;
      printf("polled succesfully key: %d, value: %d\n", key, temp->value);

      pthread_mutex_unlock(&mutex);

      return temp->value;
    }
    pthread_cond_wait(&cond, &mutex);
    temp = head;
  }
}

int count() {
  pthread_mutex_lock(&mutex);

  struct node *temp = head;
  if (head == NULL) {
    pthread_mutex_unlock(&mutex);

    return 0;
  }
  int licznik = 1;
  while (temp->next != NULL) {
    temp = temp->next;
    licznik++;
  }
  printf("count = %d\n", licznik);
  pthread_mutex_unlock(&mutex);

  return licznik;
}

void remove_node(int key) {
  pthread_mutex_lock(&mutex);

  struct node *temp_next;
  struct node *temp_previous = head;
  if (head == NULL) {
    pthread_mutex_unlock(&mutex);

    return;
  }
  if (temp_previous->key == key) {
    head = temp_previous->next;
    free(temp_previous);
    printf("removed succesfully key: %d\n", key);

    pthread_mutex_unlock(&mutex);

    return;
  }
  temp_next = temp_previous->next;

  while (temp_next->next != NULL && temp_next->key != key) {
    temp_previous = temp_next;
    temp_next = temp_next->next;
  }

  if (temp_next->key == key) {
    temp_previous->next = temp_next->next;
    free(temp_next);
    printf("removed succesfully key: %d\n", key);

    pthread_mutex_unlock(&mutex);

    return;
  }

  printf("there is no key: %d in the list\n", key);
  pthread_mutex_unlock(&mutex);
}

void print_data() {
  pthread_mutex_lock(&mutex);
  if (head==NULL)
  {
    printf("no nodes");
  }
  struct node *temp = head;
  while (temp != NULL) {
    printf("key: %d, value: %d; ", temp->key, temp->value);
    temp = temp->next;
  }
  printf("\n");

  pthread_mutex_unlock(&mutex);
}