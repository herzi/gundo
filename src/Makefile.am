# Makefile for libundo

LIB_NAME = gundo
LIB_VERSION = 0.2.0


CFLAGS = $(shell gtk-config --cflags gtk) -g -Wall -DDEBUG
#CFLAGS = $(shell gtk-config --cflags gtk) -O2
LDFLAGS = -shared $(shell gtk-config --libs gtk)
LD = gcc
AR = ar

SOURCES = $(wildcard *.c)
HHEADERS = $(wildcard *.h)
OBJECTS = $(SOURCES:.c=.o)

DOCDIR  = ../doc

SHARED_LIB = lib$(LIB_NAME).so.$(LIB_VERSION)
STATIC_LIB = lib$(LIB_NAME).a

INSTALL_LIB_DIR = $(shell gtk-config --prefix)/lib
INSTALL_INC_DIR = $(shell gtk-config --prefix)/include



all: $(STATIC_LIB) $(SHARED_LIB) 

$(SHARED_LIB): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS) 

$(STATIC_LIB): $(OBJECTS)
	$(AR) -rs $@ $^

.c.o:
	gcc -c -o $@ $< $(CFLAGS)

test: check
check:
	cd ../test;	$(MAKE) $(MAKEFLAGS) all

demo: check
	cd ../demo;	$(MAKE) $(MAKEFLAGS) all

clean:
	rm -f *.o *~ core $(SHARED_LIB) $(STATIC_LIB)

html-doc:
	doc++ --no-java-graphs --html --tables --dir $(DOCDIR) $(HHEADERS)

again: clean all

install:
	install --mode 655 $(SHARED_LIB) $(INSTALL_LIB_DIR)/
	ln -s $(INSTALL_LIB_DIR)/$(SHARED_LIB) lib$(LIB_NAME).so
	install --mode 655 $(STATIC_LIB) $(INSTALL_LIB_DIR)/
	install --mode 644 $(HHEADERS) $(INSTALL_INC_DIR)/

uninstall:
	rm -f $(INSTALL_LIB_DIR)/$(SHARED_LIB)
	rm -f $(INSTALL_LIB_DIR)/$(STATIC_LIB)
	rm -f $(INSTALL_LIB_DIR)/lib$(LIB_NAME).so
	rm -f $(HHEADERS:$(SRCDIR)/=$(INSTALL_LIB_DIR)/)

