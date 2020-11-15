SHELL = /bin/sh

# Build tools and flags
OBJS = src/main.cpp src/opengl.cpp src/socketdev.cpp src/glad.cpp
CFLAGS = -lglfw3 -lX11 -lGL -lpthread -lXrandr -lXi -ldl
CFLAGSI = -ljson-c
CC = g++

main-program: ${OBJ}
	${CC} ${OBJS} -o $@ ${CFLAGS} ${CFLAGSI}
clean:
	-rm main-program
