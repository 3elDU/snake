CC = gcc

LDFLAGS= -lncurses -ltinfo

SOURCES = src/*
OBJECTS = $(SOURCES:.c=.0)
TARGET = snake-game

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS) core

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

install:
	install -d ${DESTDIR}${PREFIX}/bin
	install -m 755 snake-game $(DESTDIR)$(PREFIX)/bin/snake-game
