Homework set 3 by Johanna Axelsson

1. Compute Sum, Min and Max of Matrix Elements (20 points)

The file matrixompB.c  and matrixompB.c contains programs that will compute the total sum of a matrix, and also find the values and coordinates for the minimum and maximum values. It will make use of openMP and declare a region as parallel and use critical zones for variables. The only difference between the files is that the b-file forces the master thread to print the result (which I think happens in my a-file as well). 

Usage (on ubuntu):
make matrixompB
./matrixompB size threads
(For task a) just change B to A)
The arguments are optional and their default values are 10000 and 10, respectively. If input is higher than that, the default values are used.

Statistics and speedup (on matrixompB): 
Due to the fact that the compiler woulnd't compile a matrix larger than 10 000 x 10 000, the times gotten were a bit low, however, I still think it is possible to reason about the results since it was a distinct difference between the measured times. 

(computed by taking sequential speed/ parallel speed)
threads 1		2		4		8
size
1000	100.00%	143.93%	69.08%	53.78%
5000	100.00%	154.48%	202.01%	186.20%
10000	100.00%	158.48%	198.78%	193.04%

As shown, it was always a good choice to use 2 threads and the speedup was logically enough double. For smaller data, it gets costly to use more threads than 2, it goes fast enough anyway. For both the 5000 and the 10000 matrix it was even more efficient to use 4 threads, but none benefitted from using 8 threads, probably because the data still is too small and it takes more time to create threads and change process for each piece of work. 


2. Quicksort (20 points)
THe file quickomp.c contains a program which generates a list of random numbers and then uses a parallel recursive quicksort to run the program. The program will first print the generated list, and then print the result. It will also print the running time of the quicksort. A short method was added to check whether the result was sorted or not. The program uses openMP and more specifically omp tasks to distribute the work between the threads. For smaller partitions, no new threads are created (since it would take more time than just completing the calls itself).

Usage:
make quickomp
./quickomp listsize threads

The arguments are optional and their default values are 100000 and 8, respectively. If input is higher than that, the default values are used (same as for matrix).

Both of the program has "DEBUG" defined for printing initial values of the matrix and the list (will also print sorted list). 

(computed by taking sequential speed/ parallel speed)
threads 1		2		4		8
size
1000	100.00%	11.08%	9.62%	8.84%
10000	100.00%	43.36%	11.87%	9.28%
100000	100.00%	46.88%	20.30%	13.16%

What we see here is actually no speedup at all, which is really strange since quicksort should be perfectly suited for parallelisation. In fact all results are worse for more threads than one. I guess it may be due to the fact that the list initially is big but that the chunks goes down in size really fast. As mentioned I drew a line for how small the interval is allowed to be when calling for new threads (set to 4 now) but that may not be enough. What is worth noticing is that the program gets "less bad" for bigger data which might imply that the workload for each thread is too small, so more time is spent on assigning threads rather than doing real computations. 