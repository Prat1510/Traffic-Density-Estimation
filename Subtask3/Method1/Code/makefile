CC=g++
LIB= `pkg-config --cflags --libs opencv`

all:
	$(CC) density.cpp -o density.out -pthread -std=c++11 $(LIB)
run:
	./density.out trafficvideo 5

clean:
	rm density.out
