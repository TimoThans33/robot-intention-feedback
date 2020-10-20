SHELL = /bin/sh

# Build tools and flags
OBJS1 = openGL-X11.c
OBJS2 = openGL-bezier.c
CFLAGS = -lX11 -lGL -lGLU
CC = gcc

openGL-bezier: ${OBJ}
	${CC} ${OBJS2} -o $@ ${CFLAGS}

openGL-example: ${OBJ}
	${CC} ${OBJS1} -o $@ ${CFLAGS}

clean:
	-rm *.o openGL-example openGL-bezier