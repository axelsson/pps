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
#define SIZE 6
#define ASIZE(x)  (sizeof(x) / sizeof(x[0]))
#define WORKERS 10
struct thread_data{
	int l[SIZE];
	int lower;
	int higher;
};

struct thread_data thread_data_array[WORKERS];
int array[SIZE];
pthread_mutex_t counter;
int count = 0;
pthread_t workerid[WORKERS];

void quickSort( int[], int, int);
int partition( int[], int, int);
void initList();

int * qs(int a[]){
	int pivot = ASIZE(a);
	printf("hej\n");
	return a;
}

int getThreadNumber(){
	pthread_mutex_lock(&counter);
	int number = count;
	count++;
	pthread_mutex_unlock(&counter);
	return number;
}

int main(int argc, char *argv[]){
	int numWorkers = 2;
	initList();
	int first = getThreadNumber();
	thread_data_array[first].l = array;
	thread_data_array[first].lower = 0;
	thread_data_array[first].higher = SIZE-1;
	pthread_create(&workerid[first], NULL, quickSort,(void *) &thread_data_array[first]);
	//quickSort(array, 0, SIZE-1);
	printf("result:\n");
  /* wait for all threads to finish*/
    for (int i = 0; i < numWorkers; i++) {
      pthread_join(workerid[i], NULL);  
    }
	for (int i = 0; i < SIZE; ++i)
	{
		printf("%d\n", array[i]);
	}

	//int h[] =qs(array);
}

void initList(){
	srand (time(NULL));
	for (int i = 0; i < SIZE; ++i)
	{
		array[i] = rand()%99;
		printf("%d\n", array[i]);
	}
	printf("size:%zu\n", ASIZE(array));
}

void * quickSort(void *threadarg){
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;
	int list[] = my_data->l;
	int low = my_data->lower;
	int high = my_data->higher;
	int pivot,j,temp,i;
	if(low<high){
		pivot = low;
		i = low;
		j = high;
		printf("values for low:%d, high:%d and pivot:%d\n", low,high, pivot);
 		//as long as low is lower than high
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
		temp=list[pivot];
		list[pivot]=list[j];
		list[j]=temp;
		int t = getThreadNumber();
		thread_data_array[t].l = list;
		thread_data_array[t].lower = low;
		thread_data_array[t].higher = j-1;
		pthread_create(&workerid[t], NULL, quickSort,(void *) &thread_data_array[t]);

		int t2 = getThreadNumber();
		thread_data_array[t2].l = list;
		thread_data_array[t2].lower = j+1;
		thread_data_array[t2].higher = high;
		pthread_create(&workerid[t2], NULL, quickSort,(void *) &thread_data_array[t2]);
		//quickSort(list,low,j-1);
		//quickSort(list,j+1,high);
	}
}
 /* void * quickSort(int list[],int low,int high)
 {
 	int pivot,j,temp,i;
 	if(low<high){
 		pivot = low;
 		i = low;
 		j = high;
 		printf("values for low:%d, high:%d and pivot:%d\n", low,high, pivot);
 		//as long as low is lower than high
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
 		temp=list[pivot];
 		list[pivot]=list[j];
 		list[j]=temp;
 		quickSort(list,low,j-1);
 		quickSort(list,j+1,high);
 	}
 }*/