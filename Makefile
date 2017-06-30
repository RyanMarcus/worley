CFLAGS=-Wall -g -pedantic -O3

worley: worley.cpp worley.h lodepng.o
	g++ $(CFLAGS) lodepng.o worley.cpp -o worley

lodepng.o: lodepng.cpp lodepng.h
	g++ $(CFLAGS) -c lodepng.cpp

.phony: clean

clean:
	rm -f worley *.o

