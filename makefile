matrix:	clean
	gcc -std=c99 -o matrix matrixsum.c -lpthread

quick: clean
	gcc -std=c99 -o quick quicksort.c -lpthread

tee: clean
	gcc -std=c99 -o tee tee.c -lpthread

birds: clean
	gcc -std=c99 -o birds hungrybirds.c -lpthread

honey: clean
	gcc -std=c99 -o honey honeybees.c -lpthread
	
matrixomp: clean
	gcc -std=c99 -o matrixomp matrixomp.c -fopenmp
test: clean
	gcc -std=c99 -o test test.c -fopenmp
clean:
	$(RM) count *.out *~ *.o *.h.gch *.exe tests.cpp utdata.txt