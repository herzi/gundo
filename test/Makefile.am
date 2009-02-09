# Makefile for Eggs


SRCDIR = ../src

CFLAGS = $(shell gtk-config --cflags gtk) -g -DDEBUG -I$(SRCDIR)
LDFLAGS = $(shell gtk-config --libs gtk)
LD = gcc

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)



all: tundo
	./tundo

tundo: tundo.o
	$(LD) $(LDFLAGS) -o $@ $^ -L$(SRCDIR) -lgundo $(LDLIBS)

clean:
	rm -f *.o *~ core

again: clean all


include depends.mk
depends.mk: $(SOURCES) $(HEADERS)
	$(CC) -M $(CFLAGS) $(SOURCES) > $@

