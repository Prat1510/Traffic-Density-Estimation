CC=g++
LIB= `pkg-config --cflags --libs opencv`

all:
	$(CC) dense.cpp -o dense.out -pthread -std=c++11 $(LIB)
	$(CC) sparse.cpp -o sparse.out -pthread -std=c++11 $(LIB)
run:
	./sparse.out trafficvideo
	./dense.out trafficvideo

utility:
	g++ utility.cpp -std=c++11
	./a.out	
clean:
	rm a.out