all: sort_test.o sort.o
	gcc -o sort_test sort_test.o sort.o

sort_test.o:sort_test.c
	gcc -o sort_test.o -c sort_test.c

sort.o:sort.c
	gcc -o sort.o -c sort.c

clean:
	rm sort.o sort_test.o sort_test
