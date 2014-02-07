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
pthread_mutex_t lock;
/*condition variables for letting the threads wait for each other*/
pthread_cond_t read;
pthread_cond_t writeStd;
pthread_cond_t writeFile;

char * line[100]; /*line will keep the input from stdin*/
void * readInput(void *arg);
void * writeOutput(void *arg);
void * writeToFile(void *arg);

/* starts three threads, one reads stdin, one writes the input to stdout
and the last one writes to a file given as an argument */
int main(int argc, char *argv[]){
	/*read filename from args*/
	char* filename = argv[1];
	/* create the threads which will run their given methods, send argument
	to the third one*/
	pthread_create(&workerid[0], NULL, readInput, (void *) 0);
	pthread_create(&workerid[1], NULL, writeOutput, (void *) 1);
	pthread_create(&workerid[2], NULL, writeToFile, (void *) filename);

	/* wait for all threads to finish*/
	for (int i = 0; i < THREADS; i++) {
		pthread_join(workerid[i], NULL);  
	}
}

/*listens for user input and then signals to the thread that will write*/
void * readInput(void *arg){
	char input[100];
	int ch;
	do{
		int i=0;
		while(EOF!=(ch=fgetc(stdin)) && ch !='\n' && i<100){
			input[i++]=ch;
		}
		input[i]='\0';
		if(*input){
            //Parsing input
			pthread_mutex_lock(&lock);
			*line = input;
			pthread_cond_signal(&read);
			pthread_mutex_unlock(&lock);
		}
	}while(ch != EOF);
}

/* waits for read to notify when new input exists. Then writes to stdout 
and continues to signal to the thread that should write to file*/
void * writeOutput(void *arg){
	while(true){
		pthread_mutex_lock(&lock);
		pthread_cond_wait(&read, &lock);
		printf("%s\n", *line);
		pthread_cond_signal(&writeStd);
		pthread_mutex_unlock(&lock);
	}
}

/*opens file (creates a new one if needed), then waits for the signal from
writeStd. When signal is receivedl, it writes to the file and then notifies 
read so that it can continue to take input.*/
void * writeToFile(void *arg){
	char* filename = (char*) arg;
	FILE *file = fopen(filename,"a+"); 
	if(!file) {
		fprintf(stderr,"Error opening file...exiting\n");
		exit(1);
	}
	//pthread_cond_signal(&writeFile);
	while(true){
		pthread_mutex_lock(&lock);
		pthread_cond_wait(&writeStd, &lock);
		fprintf(file, "%s\n", *line);
		pthread_cond_signal(&writeFile);
		pthread_mutex_unlock(&lock);
		fflush(file); /*flush, else data wont be written (since program ends with ctrl+c)*/
	}
	fclose(file);
}