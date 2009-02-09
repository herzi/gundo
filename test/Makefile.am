# Makefile for Eggs


SRCDIR = ../gundo

CFLAGS = $(shell gtk-config --cflags gtk) $(DEBUG_CFLAGS) -I$(SRCDIR)
LDFLAGS = $(shell gtk-config --libs gtk)
LD = gcc

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)



all: tundo
	./tundo

tundo: tundo.o
	$(LD) $(LDFLAGS) -o $@ $^ -L$(SRCDIR) -lgundo $(LDLIBS)

.c.o:
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o *~ core

again: clean all

