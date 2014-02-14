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
int worms;             /* shared buffer         */
int numBabies;  /*number of baby birds*/
long i;

/* main() -- read command line and create threads */
int main(int argc, char *argv[]) {
  /* thread ids and attributes */
  pthread_t pid, cid;  
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
  srand (time(NULL));

  numBabies = atoi(argv[1]);
  pthread_t babyId[numBabies];
  sem_init(&empty, SHARED, 0);  /* condition sem, init = 0 */
  sem_init(&full, SHARED, 0);   /* condition sem, init = 0  */
  sem_init(&eating, SHARED, 1); /* mutex sem, init = 1 */
  worms = 5; //initial value

  printf("main started\n");
  pthread_create(&pid, &attr, parentBird, NULL);
  /*create the given number of baby bird (consumers)*/
  for (i = 0; i < numBabies; ++i){
    pthread_create(&babyId[i], &attr, babyBird, (void * ) i);
  }
  for (int i = 0; i < numBabies; i++) {
    pthread_join(babyId[i], NULL);  
  }
}

/* Producer: the parent bird will wait for a baby to signal when the worms are finished,
  it will then gather more and signal again when there are more worms to eat */
void *parentBird(void *arg) {
  printf("Producer created\n");
  sleep(1);
  while(1){
    sem_wait(&empty); /* wait for signal from baby*/
    worms = rand()%9+1;
    printf("Parent goes out to hunt!\n");
    sleep(3);
    printf("Parent catches %d worms\n", worms);
    sem_post(&full);  /* tell baby it can eat now*/
  }
}

/* Consumers: the babies will wait for their time to eat, and if a bird discover there are no worms left,
  it will chirp for their parent for a refill. Else it will just eat a worm, pass the bowl on to
  another baby and then sleep for a while */
void *babyBird(void *arg) {
  long myId = (long) arg;
  printf("Consumer created\n");
  while(1){
    sem_wait(&eating);
    if (worms == 0){
      printf("baby %ld signals parent for more food\n", myId);
      sem_post(&empty); /* signal parent that bowl is empty*/
      sem_wait(&full);  /* wait for parent to refill */
      sem_post(&eating);  /* tell the others that it is done eating */
    }
    else{
      worms--; /* eat a worm! */
      int sleepTime = rand()%3+1;
      printf("baby %ld ate one worm and will sleep for %d sec\n", myId, sleepTime);
      sem_post(&eating);  /* tell its siblings that they can eat now*/
      sleep(sleepTime);
    }
  }
}