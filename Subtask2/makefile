CC=g++
LIB= `pkg-config --cflags --libs opencv`

all:
	$(CC) density.cpp -o density.out -pthread -std=c++11 $(LIB)

clean:
	rm density.out
	rm output.txt
