gridding: gridding.o 
	gcc -fopenmp -o gridding gridding.o -lm

gridding.o: gridding.c
	gcc -fopenmp -c gridding.c

clean:
	rm -f gridding *.o 


