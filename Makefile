CC = gcc
CFLAGS = -Wall -g -lncurses

SRC_DIR = Source
OBJ_DIR = Objects

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TARGET = tetris-cli 

CLEAN = $(OBJS) $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(CLEAN)

help:
	@echo "Usage:"
	@echo "  make            - Build the project"
	@echo "  make clean      - Clean the build artifacts"
	@echo "  make help       - Show this help message"

.PHONY: all clean help

