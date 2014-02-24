
//#define DEBUG
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#define MAXSIZE 100000
#define MAXWORKERS 8

int list[MAXSIZE]; /*the list that will be sorted*/
double start_time, end_time; /* start and end times */
int size;	/*size of the list that will be used*/
int numWorkers;	/*number of threads that will be used*/
int id;

void quickSort( int low, int high);
void initList();
void isSorted();

/* generate a list with random numbers, use openMP to run a recursive quicksort to sort it*/
int main(int argc, char *argv[]){

  /* read command line args if any */
	size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
	numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
	if (size > MAXSIZE) size = MAXSIZE;
	if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;
	omp_set_num_threads(numWorkers);

	initList();
	start_time = omp_get_wtime();
	//parallel region begins 
	#pragma omp parallel
	{
		#pragma omp single 
		quickSort(0, size-1);
	}


	end_time = omp_get_wtime();
	/* print the results and check whether the list is sorted*/
#ifdef DEBUG
	printf("Result:\n");
	for (int i = 0; i < size; ++i)
	{
		printf("%d ", list[i]);
	}
#endif
	printf("\nFinished in %g sec\n", end_time - start_time);
	isSorted();
}

/* each quickSort will do quicksearch recursively in the range given in the arguments,
 sent as a struct*/
void quickSort(int start, int end){
	int low = start;
	int high = end;
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
		//stop creating new threads if the interval is short
		if (abs(end - start) > 4){
		/* run quicksort again on the right half of the interval using omp task */
		#pragma omp task private(id)
			{
				int id = omp_get_thread_num();
				//printf("thread %d takes partition %d to %d \n", id, j+1, high);
				quickSort(j+1, high);
			}
			//quickSort(low, j-1);
		/* and another task to take care of the other half of the interval*/
		#pragma omp task private(id)
			{
				int id = omp_get_thread_num();
				//printf("thread %d takes partition %d to %d \n", id, low, j-1);
				quickSort(low, j-1);
			}
		}
		else{
			//printf("no new thread started\n");
			quickSort(j+1, high);
			quickSort(low, j-1);
		}

	}
}

/* check if the list is successfully sorted*/
void isSorted(){
	for (int i = 1; i < size; ++i)
	{
		if(list[i]<list[i-1]){
			printf("Not sorted!\n");
			return;
		}
	}
	printf("Sorted.\n");
}

/*fill the list with random numbers*/
void initList(){
	srand (time(NULL));
	for (int i = 0; i < size; ++i)
	{
		list[i] = rand()%99;
#ifdef DEBUG
		printf("%d ", list[i]);
#endif
	}
	printf("\n");
}
