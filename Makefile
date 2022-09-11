CC=clang++
CFLAGS= -g

client: client.cpp
	${CC} ${CFLAGS} -o $@ $<
