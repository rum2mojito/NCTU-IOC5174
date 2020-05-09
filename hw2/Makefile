CC = g++
#OPS = -Wall -fsanitize=address
OPS = -std=c++11 -Wall

all: sandbox sandbox.so

.PHONY: clean all run


sandbox.so: sandboxso.cc
	$(CC) -Wall  -shared -fPIC  -o $@ $< -ldl

sandbox: sandbox.cc
	$(CC) $(OPS) -o $@ $<

run:
	./sandbox

clean:
	rm sandbox sandbox.so
