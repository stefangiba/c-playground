include ../Makefile.variables

CC = gcc
EXECUTABLE_NAME = filedb
BUILD_ROOT = ../$(BUILD_DIR)/$(EXECUTABLE_NAME)
TARGET = $(BUILD_ROOT)/$(BIN_DIR)/$(EXECUTABLE_NAME)
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, $(BUILD_ROOT)/$(OBJ_DIR)/%.o, $(SRC))

run: clean build
	./$(TARGET) -f ./mynewdb.db -n
	./$(TARGET) -f ./mynewdb.db
	./$(TARGET) -f ./mynewdb.db -a "Timmy H.,123 Sheshire Ln.,120"

build: $(TARGET)

clean:
	rm -f $(BUILD_ROOT)/$(OBJ_DIR)/*.o
	rm -f $(BUILD_ROOT)/$(BIN_DIR)/*
	rm -f *.db

$(TARGET): $(OBJ)
	$(CC) -o $@ $?

$(BUILD_ROOT)/$(OBJ_DIR)/%.o: src/%.c
	$(CC) -c $< -o $@ -Iinclude
