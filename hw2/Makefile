CC = g++
#OPS = -Wall -fsanitize=address
OPS = -std=c++11
SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc=.o)

all: hw1

.PHONY: clean all run

%.o: %.cc
	$(CC) $(OPS) -c $< -o $@

hw1: $(OBJS)
	$(CC) $(OPS) -o $@ $<

run:
	./hw1

clean:
	rm *.o hw1
