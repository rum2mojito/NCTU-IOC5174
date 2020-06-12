CC := gcc
OPS := -pthread -Wall -g
TARGET := solution.c

all: solution.o

.PHONY: clean all run style

solution.o: $(TARGET)
	$(CC) $< $(OPS) -o $@

run:
	./solution.o

clean:
	rm *.o

style:
	clang-format-6.0 -style=google -i *.c
