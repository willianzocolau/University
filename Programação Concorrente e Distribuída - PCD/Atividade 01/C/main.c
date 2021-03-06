#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define SRAND_VALUE 1985
#define SIZE_GRID 2048
#define MAX_GERACOES 2000
#define MAX_THREADS 4

struct Args {
    int** grid;
    int** new_grid;
    int limite_inicial;
    int limite_final;
};

int getNeighborsLeft(int** grid, int i, int j){
  j = j - 1;
  int neighbords_left = grid[i - 1][j] + grid[i][j] + grid[i + 1][j];
  return neighbords_left;
}

int getNeighborsBottom(int** grid, int i, int j){
  i = i + 1;
  int neighbords_bottom = grid[i][j] + grid[i][j + 1];
  return neighbords_bottom;
}

int getNeighborsRight(int** grid, int i, int j){
  j = j + 1;
  int neighbords_right = grid[i][j] + grid[i - 1][j];
  return neighbords_right;
}

int getNeighborsTop(int** grid, int i, int j){
  i = i - 1;
  int neighbords_top = grid[i][j];
  return neighbords_top;
}

int getNeighbors(int** grid, int i, int j){
  int n_neighbors = 0;
  n_neighbors = n_neighbors + getNeighborsLeft(grid, i, j) + getNeighborsBottom(grid, i, j);
  n_neighbors = n_neighbors + getNeighborsRight(grid, i, j) + getNeighborsTop(grid, i, j);
  return n_neighbors;
}

void copyLeftRightColumns(int** grid){
  for (int i = 1; i <= SIZE_GRID; ++i) {
    grid[i][0] = grid[i][SIZE_GRID];
    grid[i][SIZE_GRID+1] = grid[i][1];
  }
}

void copyTopBottomLines(int** grid){
  for (int i = 1; i <= SIZE_GRID; ++i) {
    grid[0][i] = grid[SIZE_GRID][i];
    grid[SIZE_GRID+1][i] = grid[1][i];
  }
}

void copyCorners(int** grid){
  grid[0][0] = grid[SIZE_GRID][SIZE_GRID];
  grid[0][SIZE_GRID + 1] = grid[SIZE_GRID][1];
  grid[SIZE_GRID + 1][0] = grid[1][SIZE_GRID];
  grid[SIZE_GRID + 1][SIZE_GRID + 1] = grid[1][1];
}

void copyBorder(int** grid){
  copyLeftRightColumns(grid);
  copyTopBottomLines(grid);
  copyCorners(grid);
}

void newPopulation(int** grid){
  srand(SRAND_VALUE);
  int i, j;
  for (i = 1; i <= SIZE_GRID; i++){
    for (j = 1; j <= SIZE_GRID; j++){
      grid[i][j] = rand() % 2;
    }
  }
}

int countPopulation(int** grid){
  int i, j, count = 0;
  for (i = 1; i <= SIZE_GRID; i++){
    for (j = 1; j <= SIZE_GRID; j++){
      count += grid[i][j];
    }
  }
  return count;
}

int** copyPopulation(int** grid){
  int i, j;
  int **m = (int**)malloc((SIZE_GRID + 2) * sizeof(int*));
  for (i = 0; i < SIZE_GRID + 2; i++){
    m[i] = (int*) malloc((SIZE_GRID + 2) * sizeof(int));
    for (j = 0; j < SIZE_GRID + 2; j++){
      m[i][j] = grid[i][j];
    }
  }
  return m;
}

int** initialize(){
  int i, j;
  int **m = (int**)malloc((SIZE_GRID + 2) * sizeof(int*));
  for (i = 0; i < SIZE_GRID + 2; i++){
    m[i] = (int*) malloc((SIZE_GRID + 2) * sizeof(int));
    for (j = 0; j < SIZE_GRID + 2; j++){
      m[i][j] = 0;
    }
  }
  return m;
}

void liberar(int** grid){
  int i;
  for (i = 0; i < SIZE_GRID + 2; i++){
    free(grid[i]);
  }
  free(grid);
}

void DeadLife(int** grid, int** new_grid, int i, int j){
  int neighbors = getNeighbors(grid, i, j);
  int life = grid[i][j];
  if(life == 1){
    if(neighbors < 2) {
      new_grid[i][j] = 0;
    } else if (neighbors == 2 || neighbors == 3){
      new_grid[i][j] = 1;
    } else if(neighbors >= 4){
      new_grid[i][j] = 0;
    }
  } else if(life == 0 && neighbors == 3){
    new_grid[i][j] = 1;
  }
}

void* nextGeneration(void* args){
  struct Args *my_args = (struct Args*)args;
  int limite_inicial = (*my_args).limite_inicial;
  int limite_final = (*my_args).limite_final;
  int** grid = (*my_args).grid;
  int** new_grid = (*my_args).new_grid;

  int i, j;
  for (i = limite_inicial; i <= limite_final; i++){
    for (j = 1; j <= SIZE_GRID; j++){
      DeadLife(grid, new_grid, i, j);
    }
  }
}

void printPopulation(int **grid){
  printf("\n_________________________________");
  for (int i = 0; i < SIZE_GRID + 2; i++) {
    printf("\n");
    for (int j = 0; j < SIZE_GRID + 2; j++) {
      printf("%d\t", grid[i][j]);
    }
  }
}

int main() {
  int** grid = initialize();
  newPopulation(grid);

  struct timeval inicio, final2;
  int tmili;

  gettimeofday(&inicio, NULL);

  for (int i = 0; i < MAX_GERACOES; i++) {
    int incremento = SIZE_GRID / MAX_THREADS;
    int limite_inicial = 1;
    int limite_final = incremento;
    copyBorder(grid);
    int **new_grid = copyPopulation(grid);

    pthread_t t[MAX_THREADS];

    int th;
    for(th = 0; th < MAX_THREADS; th++) {
      struct Args *args = (struct Args *)malloc(sizeof(struct Args));
      args->grid = grid;
      args->new_grid = new_grid;
      args->limite_inicial = limite_inicial;
      args->limite_final = limite_final;
      pthread_create(&t[th], NULL, nextGeneration, (void *) args);
      limite_inicial += incremento;
      limite_final += incremento;
    }

    for(th = 0; th < MAX_THREADS; th++) {
      pthread_join(t[th], NULL);
    }

    liberar(grid);
    grid = new_grid;
    new_grid = NULL;
    free(new_grid);
  }

  gettimeofday(&final2, NULL);
  tmili = (int) (1000 * (final2.tv_sec - inicio.tv_sec) + (final2.tv_usec - inicio.tv_usec) / 1000);

  printf("Células vivas: %d\n", countPopulation(grid));
  printf("Tempo decorrido: %d milisegundos\n", tmili);
}

