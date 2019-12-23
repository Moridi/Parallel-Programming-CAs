CC=g++

LINK_FLAGS=-pthread

all: exer04_1.out exer04_2.out exer04_3.out

exer04_1.out: exer04_1.cpp
	$(CC) $(LINK_FLAGS) exer04_1.cpp -o exer04_1.out

exer04_2.out: exer04_2.cpp
	$(CC) $(LINK_FLAGS) exer04_2.cpp -o exer04_2.out

exer04_3.out: exer04_3.cpp
	$(CC) $(LINK_FLAGS) exer04_3.cpp -o exer04_3.out

clean:
	rm -f *.o *.out