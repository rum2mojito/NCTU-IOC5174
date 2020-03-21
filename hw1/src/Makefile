CC = g++
COPS = -Wall -fsanitize=address
OPS = -Wall
SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc=.o)

all: hw1.out

.PHONY: clean all run

%.o: %.cc
	$(CC) $(OPS) -c $< -o $@

hw1.out: $(OBJS)
	$(CC) $(OPS) -o $@ $<

run:
	./hw1.out

clean:
	rm *o hw1.out
