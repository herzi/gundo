DOCDIR=.

HHEADERS=$(wildcard ../gundo/*.h)

all: html-doc

html-doc:
if WITH_DOCPP
	$(DOCPP) --no-java-graphs --html --tables --dir $(DOCDIR) $(HHEADERS)
endif

again: clean all

