SHELL = /bin/sh

# Build tools and flags
OBJS = src/main.cpp src/opengl.cpp src/socketdev.cpp
CFLAGS = -lX11 -lGL -lGLU
CFLAGSI = -ljson-c
CC = g++

main-program: ${OBJ}
	${CC} ${OBJS} -o $@ ${CFLAGS} ${CFLAGSI}
clean:
	-rm main-program
