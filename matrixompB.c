#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 8   /* maximum number of workers */
double start_time, end_time;
int numWorkers;
int size; 
int matrix[MAXSIZE][MAXSIZE];
void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  srand (time(NULL));
  int i, j, total=0;

  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;

  omp_set_num_threads(numWorkers);

  /* initialize the matrix */
  for (i = 0; i < size; i++) {
        //printf("[ ");
    for (j = 0; j < size; j++) {
      matrix[i][j] = rand()%100;
          //  printf(" %d", matrix[i][j]);
    }
          //printf(" ]\n");
  }
  int maxX = 0, maxY = 0, max = 0, finalMax = 0, x = 0, y = 0;
  start_time = omp_get_wtime();
#pragma omp parallel for reduction (+:total) private(j, max, maxX, maxY) shared(finalMax, x, y)
  for (i = 0; i < size; i++){
    int id = omp_get_thread_num();
    //printf("thread %d working on row %d \n", id, i);
    for (j = 0; j < size; j++){
      int elem = matrix[i][j];
      if(elem > max){
        max = elem;
        maxX = i;
        maxY = j;
      }
      total += elem;
    }
#pragma omp critical
    {
    if(max > finalMax){
      //printf("thread %d updated max to: %d \n", id, max);
      finalMax = max;
      x = maxX;
      y = maxY;
    }
  }
}

#pragma omp master
{
end_time = omp_get_wtime();
printf("the total is %d\n", total);
printf("max value: %d at %d, %d\n", finalMax, x, y);
printf("it took %g seconds\n", end_time - start_time);
}
}
