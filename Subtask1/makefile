CC=g++
LIB= `pkg-config --cflags --libs opencv4`

all:
	$(CC) cam.cpp -o cam.out -pthread -std=c++11 $(LIB)

clean:
	rm cam.out 