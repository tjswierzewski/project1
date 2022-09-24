CC=clang++
CFLAGS= -g3

client: client.cpp
	${CC} ${CFLAGS} -o $@ $<
