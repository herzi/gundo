# Makefile for Eggs


SRCDIR = ../gundo

CFLAGS = $(GUNDO_CFLAGS) $(DEBUG_CFLAGS) -I$(SRCDIR)
LDFLAGS = $(GUNDO_LIBS)
LD = gcc

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)



all: sketch

sketch: sketch.o
	$(LD) $(LDFLAGS) -o $@ $^ -L$(SRCDIR) -lgundo

.c.o:
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o *~ core

again: clean all

