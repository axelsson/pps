#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define SHARED 1
#define MAX_HONEY 10

void *bear(void *);  /* the two threads */
void *bees(void *);

sem_t empty, full, fillHoney;    /* the global semaphores */
int honey;             /* shared buffer         */
int numBees;
long i;

/* main() -- read command line and create threads, then
             print result when the threads have quit */

int main(int argc, char *argv[]) {
  /* thread ids and attributes */
  pthread_t pid;  
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
  srand (time(NULL));

  numBees = atoi(argv[1]);
  numHoney = atoi(argv[2]);
  honey = MAX_HONEY;
  pthread_t beeId[numBees];
  sem_init(&empty, SHARED, 0);  /* sem empty = 1 */
  sem_init(&full, SHARED, 1);   /* sem full = 0  */
  sem_init(&fillHoney, SHARED, 1);

  printf("main started\n");
  pthread_create(&pid, &attr, bear, NULL);
  for (i = 0; i < numBees; ++i){
      //fler id
    pthread_create(&beeId[i], &attr, bees, (void * ) i);
  }
  for (int i = 0; i < numBees; i++) {
    pthread_join(beeId[i], NULL);  
  }

}

/* deposit 1, ..., numIters into the data buffer */
void *bear(void *arg) {
  printf("Consumer created\n");
  sleep(1);
  while(1){
    sem_wait(&full);
    sem_wait(&fillHoney);
    honey = 0;
    printf("Bear eats!\n");
    sem_post(&empty);
    sem_post(&fillHoney);
  }
}

/* fetch numIters items from the buffer and sum them */
void *bees(void *arg) {
  long myId = (long) arg;
  printf("Producer created\n");
  while(1){
    if (honey < MAX_HONEY){
      sem_wait(&fillHoney);
      honey++; 
      int gatheringTime = rand()%3;
      printf("bee %ld produced some honey\n", myId);
      if (honey == MAX_HONEY){
        printf("bee %ld signals bear\n", myId);
        sem_post(&full);
      }
      sem_post(&fillHoney);
      sleep(gatheringTime);
    }
    else{
      sem_wait(&empty);
    }
  }
}