//#define DEBUG
#ifndef _REENTRANT 
#define _REENTRANT 
#endif 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 10   /* maximum number of workers */

pthread_mutex_t barrier;  /* mutex lock for the barrier */
pthread_cond_t go;        /* condition variable for leaving */
int numWorkers;           /* number of workers */ 
int numArrived = 0;       /* number who have arrived */
pthread_mutex_t maxCheck; /* mutex locks for the max and min values*/
pthread_mutex_t minCheck;
pthread_mutex_t sumCheck; /* mutex lock for the total sum*/
pthread_mutex_t bagLock; /* lock for the bag of tasks*/

/*values to keep track of max and min values and their coordinates*/
int globalMaxX, globalMaxY, globalMinX, globalMinY, globalSum;
int globalMax = 0; 
int globalMin = INT_MAX;
int taskRow = 0; /*the current row for the workers to process*/
double start_time, end_time; /* start and end times */
int size, stripSize;  /* assume size is multiple of numWorkers */
int sums[MAXWORKERS]; /* partial sums */
int matrix[MAXSIZE][MAXSIZE]; /* matrix */

void *Worker(void *);
double read_timer();
int getTask();
void maxValue(int, int, int);
void minValue(int, int, int);
void updateSum(int);

/* read command line, initialize, and create threads then write results*/
    int main(int argc, char *argv[]) {
      srand (time(NULL));
      int i, j;
      long l; /* use long in case of a 64-bit system */
      pthread_attr_t attr;
      pthread_t workerid[MAXWORKERS];

  /* set global thread attributes */
      pthread_attr_init(&attr);
      pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
      
  /* initialize mutex and condition variable */
      pthread_mutex_init(&barrier, NULL);
      pthread_cond_init(&go, NULL);

  /* read command line args if any */
      size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
      numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
      if (size > MAXSIZE) size = MAXSIZE;
      if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;
      stripSize = size/numWorkers;

  /* initialize the matrix */
      for (i = 0; i < size; i++) {
       for (j = 0; j < size; j++) {
        matrix[i][j] = rand()%99;
      }
    }

  /* print the matrix */
#ifdef DEBUG
    for (i = 0; i < size; i++) {
     printf("[ ");
       for (j = 0; j < size; j++) {
         printf(" %d", matrix[i][j]);
       }
       printf(" ]\n");
     }
#endif

  /* do the parallel work: create the workers */
     start_time = read_timer();
     for (l = 0; l < numWorkers; l++)
      pthread_create(&workerid[l], &attr, Worker, (void *) l);

  /* wait for all threads to finish*/
    for (int i = 0; i < numWorkers; i++) {
      pthread_join(workerid[i], NULL);  
    }

  /* print results */
    end_time = read_timer();
    printf("The total is %d\n", globalSum);
    printf("Max: %d, at %d,%d\n", globalMax, globalMaxX, globalMaxY);
    printf("Min: %d, at %d,%d\n", globalMin, globalMinX, globalMinY);
    printf("The execution time is %g sec\n", end_time - start_time);
    pthread_exit(NULL);
  }

/* Each worker sums the values in some strips of the matrix and then updates 
  global max and min values */
  void *Worker(void *arg) {
    long myid = (long) arg;
    /*variables for local min & max, total and such*/
    int total, j, max, min, maxX, maxY, minX, minY, value;

#ifdef DEBUG
    printf("worker %d (pthread id %d) has started\n", myid, pthread_self());
#endif
    
    /*set initial values*/
    total = 0;
    max = 0;
    min = INT_MAX;

  /* get a new row to compute as long as the row is within the matrix*/
    while(true){
      int row = getTask();
      if (row >= size)
        /* break the loop if the row is outside of the matrix*/
        break;

  /* Goes through the assigned strip, keeps track of total value, max and min values. */
      for (j = 0; j < size; j++){
        value = matrix[row][j];
        if(value > max){
          max = value;
          maxX = row;
          maxY = j;
        }
        else if(value < min){
          min = value;
          minX = row;
          minY = j;
        }
        total += value;
      }
      //printf("max and min for this row %d is: %d, %d\n", row, max, min);
    }
  /* after finding the local min and max and sum, run the update methods*/
    /* these updates can be placed inside the while loop as well, but since
      we will update the local values for each row, a single update is enough
      (tested, was slower)*/
    maxValue(max, maxX, maxY);
    minValue(min, minX, minY);
    updateSum(total);
  }
  /* a reusable counter barrier */
     void Barrier() {
      pthread_mutex_lock(&barrier);
      numArrived++;
      if (numArrived == numWorkers) {
        numArrived = 0;
        pthread_cond_broadcast(&go);
      } else
      pthread_cond_wait(&go, &barrier);
      pthread_mutex_unlock(&barrier);
    }

/* Uses mutex lock then adds a value to the total sum and then unlocks it again*/
    void updateSum(int value){
      pthread_mutex_lock(&sumCheck);
      globalSum += value;
      pthread_mutex_unlock(&sumCheck);
    }

/* Uses mutex to update global max, only updates if value is bigger*/
    void maxValue(int value, int x, int y){
      pthread_mutex_lock(&maxCheck);
      if (value > globalMax){
        globalMax = value;
        globalMaxX = x;
        globalMaxY = y;
      }
      pthread_mutex_unlock(&maxCheck);
    }

/* Same as above but for global minimum value*/
    void minValue(int value, int x, int y){
      pthread_mutex_lock(&minCheck);
      if (value < globalMin){
        globalMin = value;
        globalMinX = x;
        globalMinY = y;
      }
      pthread_mutex_unlock(&minCheck);
    }

/* timer */
    double read_timer() {
      static bool initialized = false;
      static struct timeval start;
      struct timeval end;
      if( !initialized )
      {
        gettimeofday( &start, NULL );
        initialized = true;
      }
      gettimeofday( &end, NULL );
      return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
    }

/* returns a row and increments the value, mutex lock so the workers cant 
get the same row to work on*/
    int getTask(){
      pthread_mutex_lock(&bagLock);
      int newRow = taskRow;
      taskRow++;
      pthread_mutex_unlock(&bagLock);
      return newRow;
    }
