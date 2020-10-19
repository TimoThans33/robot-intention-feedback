SHELL = /bin/sh

# Build tools and flags
OBJS1 = openGL-X11.c
CFLAGS = -lX11 -lGL -lGLU
CC = gcc

openGL-example: ${OBJ}
	${CC} ${OBJS1} -o $@ ${CFLAGS}

clean:
	-rm *.o openGL-example