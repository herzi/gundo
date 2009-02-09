# Makefile for Eggs


SRCDIR = ../src

CFLAGS = $(shell gtk-config --cflags gtk) -g -DDEBUG -I$(SRCDIR)
LDFLAGS = $(shell gtk-config --libs gtk)
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

