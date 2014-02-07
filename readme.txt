Homework set 1 by Johanna Axelsson

1. Compute Sum, Min and Max of Matrix Elements (20 points)

The file matrixsum.c contains a program that will compute the total sum of a matrix, and also find the values and coordinates for the minimum and maximum values. It uses the "bag of tasks"-strategy which means the threads will take care of one strip at a time, and then take a new one if there are strips left to compute. 

Usage (on ubuntu):
make matrix
./matrix size threads

The arguments are optional and their default values are 10000 and 10, respectively. If input is higher than that, the default values are used.

2. Quicksort (20 points)
THe file quicksort.c contains a program which generates a list of random numbers and then uses a parallel recursive quicksort to run the program. The program will first print the generated list, and then print the result. It will also print the running time of the quicksort. A short method was added to check whether the result was sorted or not. 

Usage:
make quick
./quick listsize threads

The arguments are optional and their default values are 10000 and 10, respectively. If input is higher than that, the default values are used (same as for matrix).

Both of the program has "DEBUG" defined for printing initial values of the matrix and the list (will also print sorted list). 

4. The Unix tee command (20 points)
Tee.c will work just as the tee command does; except it is done with three threads. One thread listens for stdin, the second writes the input to stdout and the third one writes to the given file. The threads signal each other when they are ready for read or write.

Usage:
make tee
./tee filename

The argument filename is mandatory. 