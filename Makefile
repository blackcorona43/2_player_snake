CFLAGS = -I ./include
##LIB    = ./lib/fmod/libfmodex64.so
LFLAGS = -lrt -lX11 -lGLU -lGL -lm #-lXrandr

all: snake

snake: snake.cpp log.cpp gaponte.cpp dflores2.cpp dflores2.h rcorona.cpp rcorona.h stellez.cpp stellez.h jsingh.cpp jsingh.h
	g++ $(CFLAGS) snake.cpp log.cpp gaponte.cpp dflores2.cpp rcorona.cpp stellez.cpp jsingh.cpp libggfonts.a \
	/usr/lib/x86_64-linux-gnu/libopenal.so \
	/usr/lib/x86_64-linux-gnu/libalut.so \
	-Wall -Wextra $(LFLAGS) -o snake

clean:
	rm -f snake
	rm -f *.o

