CC = g++
#OPS = -Wall -fsanitize=address
OPS = -std=c++11 -Wall

all: sandbox sandbox.so

.PHONY: clean all test


sandbox.so: sandboxso.cc
	$(CC) $(OPS)  -shared -fPIC  -o $@ $< -ldl

sandbox: sandbox.cc
	$(CC) $(OPS) -o $@ $<

test:
	$(CC) $(OPS) -o test test.cc
	./sandbox ./test

clean:
	rm sandbox sandbox.so
