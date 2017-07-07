CFLAGS=-Wall -pedantic -O3

worley: worley.cpp worley.h lodepng.o color_funcs.o
	g++ $(CFLAGS) lodepng.o color_funcs.o worley.cpp -o worley

%.o: %.cpp %.h
	g++ $(CFLAGS) -c $<

.phony: clean

clean:
	rm -f worley *.o

