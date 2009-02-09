DOCDIR=.

HHEADERS=$(wildcard ../src/*.h)

all: html-doc

html-doc:
	doc++ --no-java-graphs --html --tables --dir $(DOCDIR) $(HHEADERS)

again: clean all

