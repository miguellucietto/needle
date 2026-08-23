CC ?= cc
PREFIX ?= /usr/local
DESTDIR ?=

CFLAGS ?= -Wall -Wextra -Wpedantic -std=c17 -g
CPPFLAGS ?= -Iinclude
LDFLAGS ?=
LDLIBS ?=
TARGET ?= needle

CFILES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c,build/%.o,$(CFILES))
DEPS = $(OBJECTS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

build/%.o: src/%.c
	@mkdir -p build
		$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

bear: all
	make clean
	bear -- make

check:
	$(CC) $(CPPFLAGS) $(CFLAGS) -fsyntax-only $(CFILES)

test: all
	@tests/test.sh ./$(TARGET)

install: all
	install -Dm755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(TARGET)

clean:
	rm -rf build $(TARGET) compile_commands.json

.PHONY: all bear check clean install test uninstall
