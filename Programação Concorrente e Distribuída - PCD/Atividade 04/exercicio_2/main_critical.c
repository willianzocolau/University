#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define SIZE 10000000
#define NUM_THREADS 8

int *vector1, *vector2;
long result;

void random_array() {
  for (int i = 0; i < SIZE; i++) {
    vector1[i] = rand() % 100;
    vector2[i] = rand() % 100;
  }
}

void scalar() {
  #pragma omp parallel for num_threads(NUM_THREADS)
  for (int i = 0; i < SIZE; i++) {
    #pragma omp critical
    result += vector1[i] * vector2[i];
  }
}

int main() {
  vector1 = (int *) malloc(sizeof(int) * SIZE);
  vector2 = (int *) malloc(sizeof(int) * SIZE);
  random_array();
  struct timeval stop, start;
  gettimeofday(&start, NULL);
  scalar();
  gettimeofday(&stop, NULL);
  double time = (double) (stop.tv_usec - start.tv_usec) / 1000000 + (double) (stop.tv_sec - start.tv_sec);
  printf("Resultado: %zu\n", result);
  printf("Tempo: %f ms", time);
  return 0;
}

