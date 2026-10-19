CC = gcc

CFLAGS = -g -O2 -Wall -Wextra -pedantic -fsanitize=address
LDFLAGS = -g -Wall -Wextra -O2 -fsanitize=address
EXEC = dis

SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib

C_SOURCES = $(filter-out $(SRC_DIR)/opcode_tables.c, $(wildcard $(SRC_DIR)/*.c)) 
SOURCES = $(C_SOURCES) $(SRC_DIR)/opcode_tables.c
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean awk run test

all: $(SRC_DIR)/opcode_tables.c $(EXEC)

$(SRC_DIR)/opcode_tables.c: $(LIB_DIR)/x86_64.isad $(SRC_DIR)/isa_decoder.awk
	awk -f $(SRC_DIR)/isa_decoder.awk $(LIB_DIR)/x86_64.isad > $(SRC_DIR)/opcode_tables.c

$(EXEC): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -o $@ -c $< $(CFLAGS)

awk:
	awk -f $(SRC_DIR)/isa_decoder.awk $(LIB_DIR)/x86_64.isad > $(SRC_DIR)/opcode_tables.c
	cat $(SRC_DIR)/opcode_tables.c

run:
	./dis test	

test:
	$(CC) test.c -o test

clean:
	-rm $(BUILD_DIR)/*.o
	-rm $(EXEC)
	-rm test
