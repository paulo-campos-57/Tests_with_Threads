#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *avg_func(void *ptr);
void *min_func(void *ptr);
void *max_func(void *ptr);

double avg;
int min = 99999;
int max = 0;

typedef struct {
  int size;
  int *values;
} data_struct;

int main(int argc, char *argv[]) {

  if (argc <= 1) {
    printf("Incorrect input.\n\n");
    exit(0);
  }

  int copy[argc - 1];
  for (int i = 0; i < (argc - 1); i++) {
    copy[i] = atoi(argv[i + 1]);
  }

  pthread_t thread1, thread2, thread3;
  int t1, t2, t3;

  data_struct ds = {argc - 1, copy};

  t1 = pthread_create(&thread1, NULL, (void *)avg_func, (void *)&ds);
  if (t1) {
    fprintf(stderr, "Error - pthread_create() return code:%d\n", t1);
    exit(EXIT_FAILURE);
  }

  t2 = pthread_create(&thread2, NULL, (void *)min_func, (void *)&ds);
  if (t2) {
    fprintf(stderr, "Error - pthread_create() return code:%d\n", t2);
    exit(EXIT_FAILURE);
  }

  t3 = pthread_create(&thread3, NULL, (void *)max_func, (void *)&ds);
  if (t3) {
    fprintf(stderr, "Error - pthread_create() return code:%d\n", t3);
    exit(EXIT_FAILURE);
  }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);

  printf("The average: %g\n", avg);
  printf("The minimum: %d\n", min);
  printf("The maximum: %d\n", max);
  exit(EXIT_SUCCESS);
}

void *avg_func(void *ptr) {
  data_struct *ds;
  ds = (data_struct *)ptr;

  avg = 0;
  for (int i = 0; i < (ds->size); i++) {
    avg += ds->values[i];
  }
  avg = avg / ds->size;
  return NULL;
}

void *min_func(void *ptr) {
  data_struct *ds;
  ds = (data_struct *)ptr;

  for (int i = 0; i < (ds->size); i++) {
    if (ds->values[i] < min) {
      min = ds->values[i];
    }
  }
  return NULL;
}

void *max_func(void *ptr) {
  data_struct *ds;
  ds = (data_struct *)ptr;

  for (int i = 0; i < (ds->size); i++) {
    if (ds->values[i] > max) {
      max = ds->values[i];
    }
  }
  return NULL;
}
