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
#define MAXSIZE 100000
#define MAXWORKERS 10

/*struct used for passing arguments to threads*/
struct thread_data{
	int lower;
	int higher;
};

/*keeps track of the data for each worker*/
struct thread_data thread_data_array[MAXWORKERS];
int list[MAXSIZE]; /*the list that will be sorted*/
double start_time, end_time; /* start and end times */
pthread_mutex_t counter; 
int count = 0; /* count how many workers are created*/
pthread_t workerid[MAXWORKERS];
int size;
int numWorkers;

void * quickSort( void *threadarg);
void quick_sort(int[], int, int);
void initList();
double read_timer();
bool isSorted();
int getThreadNumber();

/* generate a list with random numbers, use threads to run a recursive quicksort to sort it*/
int main(int argc, char *argv[]){

  /* read command line args if any */
      size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
      numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
      if (size > MAXSIZE) size = MAXSIZE;
      if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;
	//for (int k = 0; k < 1000; ++k){
		count = 0;	//reset count
		initList();
		/*create a struct to send arguments into quicksort*/
		thread_data td;
		td.lower = 0;
		td.higher = size-1;
		start_time = read_timer();
		quickSort((void*) &td);

  /* wait for all threads to finish*/
		for (int i = 0; i < count; i++) {
			pthread_join(workerid[i], NULL);  
		}
		  /* print results */
		end_time = read_timer();
	/* print the results and check whether the list is sorted*/
		printf("Result:\n");
		for (int i = 0; i < size; ++i)
		{
			printf("%d ", list[i]);
		}
		printf("\nFinished in %g sec\n", end_time - start_time);
		isSorted();
	//	if(!isSorted())
	//		break;
	//}
}

/* each quickSort will do quicksearch recursively in the range given in the arguments,
 sent as a struct*/
void *quickSort(void *threadarg){
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;
	int low = my_data->lower;
	int high = my_data->higher;
	int pivot,j,temp,i;

	if((low<high)){
		pivot = low;
		i = low;
		j = high;
 		//do as long as an interval exists
		while(i<j){
 			//while list elem is lower than pivot elem and low < high, increase high
			while((list[i]<=list[pivot])&&(i<high)){
				i++;
			}
 			//while list elem is higher than pivot, decrease high
			while(list[j]>list[pivot]){
				j--;
			}
 			//if low < high, switch places on elems at high and low
			if(i<j){
				temp=list[i];
				list[i]=list[j];
				list[j]=temp;
			}
		}
			//move pivot element to the new place
		temp=list[pivot];
		list[pivot]=list[j];
		list[j]=temp;

			/* run quicksort again on half the interval. if count == workers, 
			we will not create new thread. Instead, the current thread	will create
			a new struct and call on quicksort again*/
			if(count == numWorkers){
				thread_data newData1;
				newData1.lower = j+1;
				newData1.higher = high;
				quickSort((void *) &newData1);
			}
			/*else get the current thread number and create new thread data and new thread.*/
			else{
				int t = getThreadNumber();
				thread_data_array[t].lower = j+1;
				thread_data_array[t].higher = high;
				//printf("new thread created for interval %d to %d, id %d\n", j+1, high, &workerid[t]);
				pthread_create(&workerid[t], NULL, quickSort,(void *) &thread_data_array[t]);
			}

			/* the current thread will also take care of the other half of the interval*/
			thread_data newData;
			newData.lower = low;
			newData.higher = j-1;
			quickSort((void *) &newData);
		}
	}

/* lock that keeps track on which thread(number) should be created*/
int getThreadNumber(){
	pthread_mutex_lock(&counter);
	int number = count;
	count++;
	pthread_mutex_unlock(&counter);
	return number;
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

/* check if the list is successfully sorted*/
bool isSorted(){
	bool sorted = true;
	for (int i = 1; i < size; ++i)
	{
		if(list[i]<list[i-1])
			sorted = false;
	}
	if (!sorted){
		printf("Not sorted.\n");
		return false;
	}
	else
		printf("Sorted.\n");
	return true;
}

/*fill the list with random numbers*/
void initList(){
	srand (time(NULL));
	printf("Generated list:\n");
	for (int i = 0; i < size; ++i)
	{
		list[i] = rand()%99;
		printf("%d ", list[i]);
	}
	printf("\n");
}
