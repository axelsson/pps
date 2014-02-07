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
#define THREADS 3

pthread_t workerid[THREADS];
int numWorkers = 3;
void * readInput(void *arg);
void * writeOutput(void *arg);
void * writeFile(void *arg);

int main(int argc, char *argv[]){
	char* filename = argv[1];
	pthread_create(&workerid[0], NULL, readInput, (void *) 0);
	pthread_create(&workerid[1], NULL, writeOutput, (void *) 1);
	pthread_create(&workerid[2], NULL, writeFile, (void *) filename);
	/* wait for all threads to finish*/
    for (int i = 0; i < numWorkers; i++) {
      pthread_join(workerid[i], NULL);  
    }
}

void * readInput(void *arg){
	printf("hej\n");
}

void * writeOutput(void *arg){
	printf("hej\n");
}

void * writeFile(void *arg){
	char* file = (char[]) arg;
	printf("hej\n");
}