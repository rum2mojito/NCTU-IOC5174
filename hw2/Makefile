CC = g++
# OPS = -std=c++11 -Wall -D DEBUG
OPS = -std=c++11 -Wall

all: sandbox sandbox.so

.PHONY: clean all test

sandbox.so: sandboxso.cc
	$(CC) $(OPS) -shared -fPIC $< -o $@ -ldl

sandbox: sandbox.cc
	$(CC) $(OPS) -o $@ $<

test:
	gcc sample.c -o sample1
	gcc sample.c -o sample2 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64
	./sandbox ./sample1
	./sandbox ./sample2
	rm sample1 sample2

clean:
	rm sandbox sandbox.so
