/* Basic quicksort implementation Usage: qs arraySize numThreads */

#ifndef _REENTRANT 
#define _REENTRANT 
#endif 
#include <pthread.h> 
#include <omp.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <time.h> 
#include <sys/time.h> 
#define MAXSIZE 100000000 /* maximum array size */ 
#define MAXTHREADS 10 
#define RUNS 5 // how many times to run the sorting algorithm (used to get a median execution time)

int comp (const void * elem1, const void * elem2) 
{ int f = *((double*)elem1);
	int s = *((double*)elem2); 
	return (f > s) - (f < s); }


/* timer */ double read_timer() 
	{ static bool initialized = false;
		static struct timeval start;
		struct timeval end;
		if( !initialized ) 
			{ gettimeofday( &start, NULL ); initialized = true; } gettimeofday( &end, NULL );
			 return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec); 
			}

/* Thread argument, contains the range which a thread should sort*/ typedef struct { int first; int last; }range;

		void *Thread (void *);
		 void parallell_qs ();
		  void printArray ();
		   bool isSorted (int);

		long size = MAXSIZE;
		pthread_attr_t attr;
		int array[MAXSIZE];
		int num_threads;

		int main(int argc, char const *argv[]) { 
			int i, j; /* set global thread attributes */ 
			pthread_attr_init(&attr); 
			pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

/* read command line args if any */ size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
			if (size > MAXSIZE) size = MAXSIZE;
			num_threads = (argc > 1)? atoi(argv[2]) : MAXTHREADS;

			omp_set_num_threads(num_threads);

			double times[RUNS];
			for (j = 0;j<RUNS;j++)
 { /* Initialize array to be sorted*/ 
				srand((unsigned)time(NULL)); 
			for (i = 0;i < size;i++){ array[i] = rand ()%99;
			} double startTime = read_timer ();
   /*sort the array*/ 
   #pragma omp parallel 
   #pragma omp single 
			parallell_qs (0, size-1);
    /*read timer*/ double endTime = read_timer ();
			double elapsedTime = endTime - startTime;
			times[j] = elapsedTime; } 

//sort times array and take the median time 
			qsort (times, RUNS, sizeof(*times), (__compar_fn_t)comp);
			double median_time = times[RUNS/2];
			if (isSorted (size)){ printf("Using %d threads - Sorted %ld elements in %g s \n", num_threads, size, median_time);
		} else{ printf ("Error: Array unsorted!\n");
	} if (size <= 200) printArray ();
	return 0; }

	void parallell_qs(int first, int last){ 
		if (last - first < 1) return;
		int pivotIndex = first+((last - first) / 2);
 //Use middle element as pivot 
		int pivot = array[pivotIndex]; int i;
		int left = first;
		int right = last;
  //Sort elements so that elements smaller than the pivot is to the left, 
  //and elements larger than the pivot to the right. 
		while (left <= right){ 
			while (array[left] < pivot) 
				left++;
			while (array[right] > pivot) 
				right--; 
			if (left <= right){ 
				int tmp = array[left];
				array[left] = array[right];
				array[right] = tmp; 
				left++; 
				right--; }
      } //recurse on the smaller lists 
      //Dont split the thread into two if the list is too small 
      if (last - first > num_threads){ 
      #pragma omp task 
      	parallell_qs (first, right);
      	 #pragma omp task 
      	parallell_qs (left, last);
      }else{ parallell_qs (first, right); 
      	parallell_qs (left, last); }
      }

      void printArray ()
      { int i;
      	for (i = 0;i<size;i++) printf("%d,",array[i]);
      		printf("]\n");
      }

      bool isSorted (int size){ 
      	int i;
      	for (i=1;i<size;i++){ 
      		if (array[i] < array[i-1]) return false;
      	} return true;
      }