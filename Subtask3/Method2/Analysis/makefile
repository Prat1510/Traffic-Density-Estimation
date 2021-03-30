CC=g++
LIB= `pkg-config --cflags --libs opencv`

all:
	$(CC) error.cpp -o error.out -pthread -std=c++11 $(LIB)
error:
	./error.out
graph:
	python3 graph.py
clean:
	rm *.jpg
	rm *.txt
	rm error.out