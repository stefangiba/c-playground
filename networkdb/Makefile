include ../Makefile.variables

BUILD_ROOT = ../$(BUILD_DIR)/networkdb

SERVER_TARGET = $(BUILD_ROOT)/$(BIN_DIR)/networkdb
SERVER_SRC = $(wildcard src/server/*.c)
SERVER_OBJ = $(patsubst src/%.c, $(BUILD_ROOT)/$(OBJ_DIR)/%.o, $(SERVER_SRC))

CLIENT_TARGET = $(BUILD_ROOT)/$(BIN_DIR)/networkdb-client
CLIENT_SRC = $(wildcard src/client/*.c)
CLIENT_OBJ = $(patsubst src/%.c, $(BUILD_ROOT)/$(OBJ_DIR)/%.o, $(CLIENT_SRC))

build: clean test

test: compile
	echo "TODO: implement tests"

compile: $(SERVER_TARGET) $(CLIENT_TARGET)

clean:
	rm -f $(BUILD_ROOT)/$(OBJ_DIR)/**/*.o
	rm -f $(BUILD_ROOT)/$(BIN_DIR)/*

$(SERVER_TARGET): $(SERVER_OBJ)
	$(CC) -o $@ $?

$(BUILD_ROOT)/$(OBJ_DIR)/server/%.o: src/server/%.c
	$(CC) -c $< -o $@ -Iinclude

$(CLIENT_TARGET): $(CLIENT_OBJ)
	$(CC) -o $@ $?

$(BUILD_ROOT)/$(OBJ_DIR)/client/%.o: src/client/%.c
	$(CC) -c $< -o $@ -Iinclude