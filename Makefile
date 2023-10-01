CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
LIB_NAME = ezcsv

SRC_DIR = src
BUILD_DIR = build
LIB_DIR = /usr/local/lib
INCLUDE_DIR = /usr/local/include

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

all: $(BUILD_DIR) $(LIB_NAME)

$(LIB_NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) -shared -o $(BUILD_DIR)/lib$(LIB_NAME).so $(OBJ_FILES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(BUILD_DIR):
	mkdir $@

install: $(LIB_NAME)
	install -d $(LIB_DIR)
	install -m 644 $(BUILD_DIR)/lib$(LIB_NAME).so $(LIB_DIR)
	install -d $(INCLUDE_DIR)
	install -m 644 $(SRC_DIR)/ezcsv.h $(INCLUDE_DIR)

uninstall:
	rm -f $(LIB_DIR)/lib$(LIB_NAME).so
	rm -f $(INCLUDE_DIR)/ezcsv.h

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all install uninstall clean
