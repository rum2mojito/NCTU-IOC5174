CC = g++
#OPS = -Wall -fsanitize=address
OPS = -std=c++11
SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc=.o)
TARGET = hw2

all: hw2

.PHONY: clean all run

%.o: %.cc
	$(CC) $(OPS) -c $< -o $@

hw2: $(OBJS)
	$(CC) $(OPS) -o $@ $<

run:
	./$(TARGET)

clean:
	rm *.o $(TARGET)
