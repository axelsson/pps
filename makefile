matrix:	clean
	g++ -o matrix matrixsum.c -lpthread


quick: clean
	g++ -o quick quicksort.c -lpthread

clean:
	$(RM) count *.out *~ *.o *.h.gch tests.cpp utdata.txt