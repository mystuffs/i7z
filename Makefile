# Makefile for i7z, GPL v2, License in COPYING

CFLAGS ?= -O3 -Wall -Wextra
CFLAGS += -D_GNU_SOURCE
LIBS += -lncurses -lpthread -lrt -lm
INCLUDEFLAGS = 
CC ?= gcc

BIN	= i7z
SRC	= $(wildcard src/*.c)
OBJ	= $(SRC:.c=.o)

prefix ?= /usr/local
sbindir = $(prefix)/sbin/
docdir = $(prefix)/share/doc/$(BIN)/
mandir ?= $(prefix)/share/man/

bin: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LIBS) 

# perfmon-bin: message $(OBJ)
# 	$(CC) $(CFLAGS) -o $(PERFMON-BIN) perfmon-i7z.c helper_functions.c $(LIBS)

clean:
	rm -f src/*.o $(BIN)

distclean: clean
	rm -f *~ \#*

install:  $(BIN)
	install -D -m 0644 doc/i7z.man $(DESTDIR)$(mandir)man1/i7z.1
	install -D -m 755 $(BIN) $(DESTDIR)$(sbindir)$(BIN)
