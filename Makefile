CC=g++
CFLAGS= -g3 -O0 -D_GLIBCXX_DEBUG -std=c++17

client: client.cpp evaluate.cpp
	${CC} ${CFLAGS} -L/usr/lib/ssl/lib -lssl -lcrypto -o $@ $^

eval: check.cpp evaluate.cpp
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm client