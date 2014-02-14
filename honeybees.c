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
int numBees;  /* number of bees working*/
long i;

/* main() -- read command line and create threads */
int main(int argc, char *argv[]) {
  /* thread ids and attributes */
  pthread_t pid;  
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
  srand (time(NULL));

  numBees = atoi(argv[1]);
  honey = 0;
  pthread_t beeId[numBees];
  sem_init(&empty, SHARED, 0);  /* condition sem init = 0 */
  sem_init(&full, SHARED, 0);   /* condition sem init = 0 */
  sem_init(&fillHoney, SHARED, 1);  /* mutex sem init = 1 */

  printf("main started\n");
  pthread_create(&pid, &attr, bear, NULL);
  /* create the given number of bees (producers)*/
  for (i = 0; i < numBees; ++i){
    pthread_create(&beeId[i], &attr, bees, (void * ) i);
  }
  for (int i = 0; i < numBees; i++) {
    pthread_join(beeId[i], NULL);  
  }

}
/* Consumer: the bear will wait for a bee to wake it up, then eat all the honey and 
 tell the bee to continue to produce honey */
void *bear(void *arg) {
  printf("Consumer created\n");
  sleep(1);
  while(1){
    sem_wait(&full);
    honey = 0;
    printf("Bear eats!\n");
    sem_post(&empty);
  }
}

/* Producers: The bees will wait for their turn to put honey into the pot. If the pot is full, 
  the bee will signal the bear and then wait for the bear to say it needs more. It will then
  let the other bees continue to put honey in the pot. If the put is not full it will leave its honey.*/
void *bees(void *arg) {
  long myId = (long) arg;
  printf("Producer created\n");
  while(1){
    /* wait for its turn*/
    sem_wait(&fillHoney);
    if (honey == MAX_HONEY){
      printf("bee %ld signals bear\n", myId);
      sem_post(&full);  /* tell bear honey pot is full*/
      sem_wait(&empty); /* wait for it to be empty again*/
      sem_post(&fillHoney); /* tell other bees to continue*/
    }
    else{
      honey++; /* put some honey in the pot*/
      int gatheringTime = rand()%3;
      printf("bee %ld produced some honey\n", myId);
      sem_post(&fillHoney); /* tell the other bees the pot is "free" */
      sleep(gatheringTime);
    }
  }
}