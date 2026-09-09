CC = gcc

CFLAGS = -g -O2 -Wall -Wextra -pedantic -fsanitize=address
LDFLAGS = -g -Wall -Wextra -O2 -fsanitize=address
EXEC = dis
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: test $(EXEC)

%.o: %.c %.h
	gcc -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJECTS)
	gcc -o $@ $< $(LDFLAGS)

run:
	./dis test

test:
	gcc test.c -o test

clean:
	-rm *.o
	-rm $(EXEC)
	-rm test
