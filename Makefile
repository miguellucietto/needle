CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c17 -ggdb -Iinclude -MMD -MP
TARGET ?= needle

CFILES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c,build/%.o,$(CFILES))
DEPS = $(OBJECTS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

bear: all
	make clean
	bear -- make

clean:
	rm -rf build $(TARGET)

.PHONY: all clean
