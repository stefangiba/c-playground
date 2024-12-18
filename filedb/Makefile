include ../Makefile.variables

EXECUTABLE_NAME = filedb
BUILD_ROOT = ../$(BUILD_DIR)/$(EXECUTABLE_NAME)
TARGET = $(BUILD_ROOT)/$(BIN_DIR)/$(EXECUTABLE_NAME)
SRC = $(call rwildcard,src,*.c)
OBJ = $(patsubst src/%.c, $(BUILD_ROOT)/$(OBJ_DIR)/%.o, $(SRC))

build: clean test

test: compile
	./$(TARGET) -f ./mynewdb.db -n
	./$(TARGET) -f ./mynewdb.db
	./$(TARGET) -f ./mynewdb.db -a "Timmy H.,123 Sheshire Ln.,120"
	./$(TARGET) -f ./mynewdb.db -l

compile: $(TARGET)

clean:
	rm -f $(BUILD_ROOT)/$(OBJ_DIR)/**/*.o
	rm -f $(BUILD_ROOT)/$(BIN_DIR)/*
	rm -f *.db

$(TARGET): $(OBJ)
	$(CC) -o $@ $?

$(BUILD_ROOT)/$(OBJ_DIR)/%.o: src/%.c
	$(CC) -c $< -o $@ -Iinclude
