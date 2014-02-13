#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define SHARED 1

void *parentBird(void *);  /* the two threads */
void *babyBird(void *);

sem_t empty, full, eating;    /* the global semaphores */
int data;             /* shared buffer         */
int numIters;
int numBabies;
long i;

/* main() -- read command line and create threads, then
             print result when the threads have quit */

int main(int argc, char *argv[]) {
  /* thread ids and attributes */
  pthread_t pid, cid;  
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
  srand (time(NULL));

  numBabies = atoi(argv[1]);
  numIters = atoi(argv[2]);
  pthread_t babyId[numBabies];
  sem_init(&empty, SHARED, 1);  /* sem empty = 1 */
  sem_init(&full, SHARED, 0);   /* sem full = 0  */
  sem_init(&eating, SHARED, 1);

  printf("main started\n");
  pthread_create(&pid, &attr, parentBird, NULL);
  for (i = 0; i < numBabies; ++i){
      //fler id
    pthread_create(&babyId[i], &attr, babyBird, (void * ) i);
  }

  pthread_join(pid, NULL);
  pthread_join(cid, NULL);
  printf("main done\n");
}

/* deposit 1, ..., numIters into the data buffer */
void *parentBird(void *arg) {
  printf("Producer created\n");
  sleep(1);
  while(1){
    sem_wait(&empty);
    sem_wait(&eating);
    data = rand()%9+1;
    printf("Parent goes out to hunt!\n");
    sleep(3);
    printf("Parent catches %d worms\n", data);
    sem_post(&full);
    sem_post(&eating);
  }
}

/* fetch numIters items from the buffer and sum them */
void *babyBird(void *arg) {
  long myId = (long) arg;
  printf("Consumer created\n");
  while(1){
    if (data > 0){

      sem_wait(&eating);
      data--; 
      int sleepTime = rand()%3+1;
      printf("baby %ld ate one worm and will sleep for %d sec\n", myId, sleepTime);
      if (data == 0){
        printf("baby %ld signals parent for more food\n", myId);
        sem_post(&empty);
      }
      sem_post(&eating);
      sleep(sleepTime);
    }
    else{
      sem_wait(&full);
    }
  }
}