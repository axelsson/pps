matrix:	clean
	g++ -o matrix matrixsum.c -lpthread
clean:
	$(RM) count *.out *~ *.o *.h.gch tests.cpp utdata.txt


testmatrix: clean
	g++ -o matrix testMatrix.c -lpthread