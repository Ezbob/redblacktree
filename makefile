CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
OBJECTS = RBTree.o RBT_tester.o
HEADERS = RBTree.h
EXE_NAME = RTBtest

.DEFAULT_GOAL := $(EXE_NAME)

$(EXE_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJECTS): $(HEADERS)

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(EXE_NAME)
