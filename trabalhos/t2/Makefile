.SECONDARY:

CC=gcc
CFLAGS=-Wall -Wextra -ansi -O3 -g -Iinclude -MP -MD

COMMON_DIR=src
ENTRYPOINTS_DIR=entrypoints
BIN_DIR=bin

COMMON_CFILES=$(wildcard $(COMMON_DIR)/*.c)
COMMON_OBJECTS=$(patsubst %.c,%.o,$(COMMON_CFILES))
COMMON_DEPS=$(patsubst %.c,%.d,$(COMMON_CFILES))

ENTRYPOINTS_CFILES=$(wildcard $(ENTRYPOINTS_DIR)/*.c)
ENTRYPOINTS_OBJECTS=$(patsubst %.c,%.o,$(ENTRYPOINTS_CFILES))
ENTRYPOINTS_DEPS=$(patsubst %.c,%.d,$(ENTRYPOINTS_CFILES))

BINARIES=$(patsubst $(ENTRYPOINTS_DIR)/%.c,$(BIN_DIR)/%,$(ENTRYPOINTS_CFILES))

all: $(BINARIES)

$(BIN_DIR)/%:$(ENTRYPOINTS_DIR)/%.o $(COMMON_OBJECTS)
	$(CC) -o $@ $^ -lm

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(COMMON_OBJECTS) $(COMMON_DEPS) $(ENTRYPOINTS_OBJECTS) $(ENTRYPOINTS_DEPS) $(BINARIES)

-include $(COMMON_DEPS) $(ENTRYPOINTS_DEPS)

.PHONY: all clean