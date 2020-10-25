SHELL = /bin/sh

# Build tools and flags
OBJS1 = openGL-X11.c
OBJS2 = openGL-drawArray/main.cpp
OBJS3 = openGL-threading/main.cpp
CFLAGS = -lX11 -lGL -lGLU
CFLAGSI = -lpthread
CC = g++

openGL-thread: ${OBJ}
	${CC} ${OBJS3} -o $@ ${CFLAGS} ${CFLAGSI}
openGL-array: ${OBJ}
	${CC} ${OBJS2} -o $@ ${CFLAGS}

openGL-example: ${OBJ}
	${CC} ${OBJS1} -o $@ ${CFLAGS}

clean:
	-rm openGL-example openGL-array openGL-thread