Homework set 1 by Johanna Axelsson

1. Compute Sum, Min and Max of Matrix Elements (20 points)

The file matrixsum.c contains a program that will compute the total sum of a matrix, and also find the values and coordinates for the minimum and maximum values. It uses the "bag of tasks"-strategy which means the threads will take care of one strip at a time, and then take a new one if there are strips left to compute. 

Compile the code by typing "make matrix" and run the program by typing "./matrix". The program can take two arguments, the first being the size of the matrix and the second being the number of threads you want to use. As these arguments are optional, the default values are size 10000 with a number of 10 workers and arguments higher than that will not be used. 
Example that will compile and run the program with a 100x100 matrix and two threads:
make matrix
./matrix 100 2

2. Quicksort (20 points)
THe file quicksort.c contains a program which generates a list of random numbers and then uses a parallel recursive quicksort to run the program. The program will first print the generated list, and then print the result. It will also print the running time of the quicksort. A short method was added to check whether the result was sorted or not. 

Compile by typing "make quick" and run the program with "./quick". This program also has two optional parameters, first the size of the list to be sorted, and second the number of threads to be used. Default values are size 10000 and 10 threads and arguments with higher values than that will not be used. The numbers in the list to be sorted are randomly generated. 
Example that will sort a list of 50 numbers with two threads:
make quick
./quick 50 2
