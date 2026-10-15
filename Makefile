CC = gcc

CFLAGS = -g -O2 -Wall -Wextra -pedantic -fsanitize=address
LDFLAGS = -g -Wall -Wextra -O2 -fsanitize=address
EXEC = dis

SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean

all: test $(EXEC)

$(SRC_DIR)/opcode_tables.c: $(LIB_DIR)/x86_64.isad $(SRC_DIR)/isa_decoder.awk
	awk -f $(SRC_DIR)/isa_decoder.awk $(LIB_DIR)/x86_64.isad > $(SRC_DIR)/opcode_tables.c

$(EXEC): $(OBJECTS) $(SRC_DIR)/opcode_tables.c
	$(CC) $(OBJECTS) $(SRC_DIR)/opcode_tables.c -o $(EXEC) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

run:
	./dis test	
	cat src/opcode_tables.c

test:
	$(CC) test.c -o test

clean:
	-rm $(BUILD_DIR)/*.o
	-rm $(EXEC)
	-rm test
