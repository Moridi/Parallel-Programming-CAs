CC=g++

LINK_FLAGS=-fopenmp 

all: exer01_1 exer01_2

exer01_1: exer01_1.cpp
	$(CC) $(LINK_FLAGS) exer01_1.cpp -o exer01_1.out

exer01_2: exer01_2.cpp
	$(CC) $(LINK_FLAGS) exer01_2.cpp -o exer01_2.out

clean:
	rm -f *.o *.out