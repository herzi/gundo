# Makefile for libundo

LIB_NAME = gundo
LIB_VERSION = 0.2.0


CFLAGS = $(shell gtk-config --cflags gtk) -g -Wall -DDEBUG
#CFLAGS = $(shell gtk-config --cflags gtk) -O2
LDFLAGS = -shared $(shell gtk-config --libs gtk)
LD = gcc
AR = ar

SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJECTS = $(SOURCES:.c=.o)

SHARED_LIB = lib$(LIB_NAME).so.$(LIB_VERSION)
STATIC_LIB = lib$(LIB_NAME).a

INSTALL_LIB_DIR = $(shell gtk-config --prefix)/lib
INSTALL_INC_DIR = $(shell gtk-config --prefix)/include



all: $(STATIC_LIB) $(SHARED_LIB) 

$(SHARED_LIB): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS) 

$(STATIC_LIB): $(OBJECTS)
	$(AR) -rs $@ $^

test: check
check:
	cd ../test;	$(MAKE) $(MAKEFLAGS) all

demo: check
	cd ../demo;	$(MAKE) $(MAKEFLAGS) all

clean:
	rm -f *.o *~ core $(SHARED_LIB) $(STATIC_LIB)
	cd ../test; $(MAKE) $(MAKEFLAGS) clean

again: clean all

install:
	install --mode 655 $(SHARED_LIB) $(INSTALL_LIB_DIR)/
	ln -s $(INSTALL_LIB_DIR)/$(SHARED_LIB) lib$(LIB_NAME).so
	install --mode 655 $(STATIC_LIB) $(INSTALL_LIB_DIR)/
	install --mode 644 $(HEADERS) $(INSTALL_INC_DIR)/

uninstall:
	rm -f $(INSTALL_LIB_DIR)/$(SHARED_LIB)
	rm -f $(INSTALL_LIB_DIR)/$(STATIC_LIB)
	rm -f $(INSTALL_LIB_DIR)/lib$(LIB_NAME).so
	rm -f $(HEADERS:$(SRCDIR)/=$(INSTALL_LIB_DIR)/)


include depends.mk
depends.mk: $(SOURCES) $(HEADERS)
	$(CC) -M $(CFLAGS) $(SOURCES) > $@



