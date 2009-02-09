# Makefile for libundo

LIB_NAME = gundo
LIB_VERSION = 2:0:2


CFLAGS = $(shell gtk-config --cflags gtk) $(DEBUG_CFLAGS)
LDFLAGS = -shared $(shell gtk-config --libs gtk)
LD = gcc
AR = ar

SOURCES = $(wildcard *.c)
HHEADERS = $(wildcard *.h)
OBJECTS = $(SOURCES:.c=.o)
LOBJECTS = $(SOURCES:.c=.lo)

DOCDIR  = ../doc

LIBTOOL_LIB = lib$(LIB_NAME).la
SHARED_LIB = lib$(LIB_NAME).so.0.2.0
STATIC_LIB = lib$(LIB_NAME).a

INSTALL_LIB_DIR = $(shell gtk-config --prefix)/lib
INSTALL_INC_DIR = $(shell gtk-config --prefix)/include

all: $(STATIC_LIB) $(SHARED_LIB)

$(SHARED_LIB): $(LIBTOOL_LIB)

$(LIBTOOL_LIB): $(LOBJECTS) $(OBJECTS)
	$(LIBTOOL) --mode=link $(LD) -o $@ $(LOBJECTS) $(LDFLAGS) -rpath /usr/local/lib -version-info $(LIB_VERSION)

$(STATIC_LIB): $(LIBTOOL_LIB)

.c.o:
	gcc -c -o $@ $< $(CFLAGS)

.c.lo:
	$(LIBTOOL) --mode=compile gcc -c -o $@ $< $(CFLAGS)

test: check
check:
	cd ../test;	$(MAKE) $(MAKEFLAGS) all

demo: check
	cd ../demo;	$(MAKE) $(MAKEFLAGS) all

clean:
	rm -f *.o *~ core $(SHARED_LIB) $(STATIC_LIB)

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

