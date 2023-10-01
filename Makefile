CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
LIB_NAME = ezcsv
TEST_EXECUTABLE = test_ezcsv

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
LIB_DIR = /usr/local/lib
INSTALL_INCLUDE_DIR = /usr/local/include

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

TEST_DIR = tests
TEST_SRC_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SRC_FILES))

CHECK_LIBS = -lcheck -lm -lpthread -lrt -lsubunit

all: $(BUILD_DIR) $(LIB_NAME)

$(LIB_NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) -shared -o $(BUILD_DIR)/lib$(LIB_NAME).so $(OBJ_FILES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -fPIC -I$(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR):
	mkdir $@

install: $(LIB_NAME)
	install -d $(LIB_DIR)
	install -m 644 $(BUILD_DIR)/lib$(LIB_NAME).so $(LIB_DIR)
	install -d $(INSTALL_INCLUDE_DIR)
	install -m 644 $(INCLUDE_DIR)/ezcsv.h $(INSTALL_INCLUDE_DIR)

uninstall:
	rm -f $(LIB_DIR)/lib$(LIB_NAME).so
	rm -f $(INSTALL_INCLUDE_DIR)/ezcsv.h

test: $(BUILD_DIR) $(LIB_NAME) $(TEST_EXECUTABLE)
	./$(BUILD_DIR)/$(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_OBJ_FILES)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(TEST_EXECUTABLE) $(TEST_OBJ_FILES) $(OBJ_FILES) $(CHECK_LIBS)

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all install uninstall clean test
