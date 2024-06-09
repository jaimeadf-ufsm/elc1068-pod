.SECONDARY:

CC=gcc
CFLAGS=-Wall -Wextra -ansi -O3 -g -Iinclude -MP -MD

COMMON_DIR=src
TOOLS_DIR=tools
BIN_DIR=bin

COMMON_CFILES=$(wildcard $(COMMON_DIR)/*.c)
COMMON_OBJECTS=$(patsubst %.c,%.o,$(COMMON_CFILES))
COMMON_DEPS=$(patsubst %.c,%.d,$(COMMON_CFILES))

TOOLS_CFILES=$(wildcard $(TOOLS_DIR)/*.c)
TOOLS_OBJECTS=$(patsubst %.c,%.o,$(TOOLS_CFILES))
TOOLS_DEPS=$(patsubst %.c,%.d,$(TOOLS_CFILES))

BINARIES=$(patsubst $(TOOLS_DIR)/%.c,$(BIN_DIR)/%,$(TOOLS_CFILES))

all: $(BINARIES)

$(BIN_DIR)/%:$(TOOLS_DIR)/%.o $(COMMON_OBJECTS)
	$(CC) -o $@ $^ -lm

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(COMMON_OBJECTS) $(COMMON_DEPS) $(TOOLS_OBJECTS) $(TOOLS_DEPS) $(BINARIES)

-include $(COMMON_DEPS) $(TOOLS_DEPS)

.PHONY: all clean