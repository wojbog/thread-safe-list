#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

extern pthread_mutex_t mutex;
extern pthread_cond_t cond;

struct node {
  int key;
  int value;
  struct node *next;
};
extern struct node *head;

struct node *first();

struct node *next(struct node *obj);

struct node *last();

void insert(int key, int value);

int get(int key);

int poll(int key);

int count();

void remove_node(int key);

void print_data();