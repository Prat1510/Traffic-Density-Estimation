CC=g++
LIB= `pkg-config --cflags --libs opencv`

all:
	$(CC) utility.cpp -o utility.out -pthread -std=c++11 $(LIB)
utility:
	./utility.out 5
graph:
	python3 graph.py
clean:
	rm *.txt
	rm utility.out