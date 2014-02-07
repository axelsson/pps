matrix:	clean
	gcc -std=c99 -o matrix matrixsum.c -lpthread

quick: clean
	gcc -std=c99 -o quick quicksort.c -lpthread

tee: clean
	gcc -std=c99 -o tee tee.c -lpthread
	
clean:
	$(RM) count *.out *~ *.o *.h.gch tests.cpp utdata.txt