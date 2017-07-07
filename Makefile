CFLAGS=-Wall -pedantic -O3

worley: worley.cpp worley.h trajectory.o lodepng.o color_funcs.o
	g++ $(CFLAGS) lodepng.o color_funcs.o trajectory.o worley.cpp -o worley

%.o: %.cpp %.h
	g++ $(CFLAGS) -c $<

anim.mp4: top.mp4 bot.mp4
	ffmpeg -i top.mp4 -i bot.mp4 -filter_complex vstack anim.mp4

top.mp4: c12.mp4 out_t3.mp4
	ffmpeg -i c12.mp4 -i out_t3.mp4 -filter_complex hstack top.mp4

c12.mp4: out_t1.mp4 out_t2.mp4
	ffmpeg -i out_t1.mp4 -i out_t2.mp4 -filter_complex hstack c12.mp4

bot.mp4: c45.mp4 out_t6.mp4
	ffmpeg -i c45.mp4 -i out_t6.mp4 -filter_complex hstack bot.mp4

c45.mp4: out_t4.mp4 out_t5.mp4
	ffmpeg -i out_t4.mp4 -i out_t5.mp4 -filter_complex hstack c45.mp4

out_t%.mp4: worley
	./worley -w 800 -h 800 -a -n 200 -t $* $@


.phony: clean

clean:
	rm -f worley *.o *.png *.mp4 *.gif *.mkv

