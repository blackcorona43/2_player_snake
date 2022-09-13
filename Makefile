CFLAGS = -I ./include
##LIB    = ./lib/fmod/libfmodex64.so
LFLAGS = -lrt -lX11 -lGLU -lGL -lm #-lXrandr

all: snake

snake:  snake.cpp dflores2.cpp gaponte.cpp stellez.cpp rcorona.cpp jsingh.cpp log.cpp
	g++ $(CFLAGS) dflores2.cpp gaponte.cpp stellez.cpp rcorona.cpp 
	jsingh.cpp snake.cpp log.cpp libggfonts.a \
	/usr/lib/x86_64-linux-gnu/libopenal.so \
	/usr/lib/x86_64-linux-gnu/libalut.so \
	-Wall -Wextra $(LFLAGS) -o snake

clean:
	rm -f snake
	rm -f *.o

