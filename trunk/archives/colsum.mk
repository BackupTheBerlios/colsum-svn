CC= gcc
STRIP = strip
EXE = colsum
LIB = -lm -lcf
OBJECTS = colsum.o colmod.o
CFLAGS = -O2 -Wall -I../cflib/include -L../cflib/libmgw
#CFLAGS = -Wall -mshort -fomit-frame-pointer $(DEBUG)
NROFF = /bin/nroff -man -T$(TERM)
COLMANPATH = /usr/man/man1/
COLBINPATH = /usr/local/bin/
ARCHIV = colsum9
FILES= colsum.c colmod.c colsum.h colsum.1 colsum.man colsum \
       strutil.c makefile makefile.unx makefile.os2

all: $(EXE) man

clean:
	rm -rf *.o *.d $(EXE)

man: colsum.man

$(EXE): $(OBJECTS) colsum.h
	$(CC) $(CFLAGS) $(OBJECTS) $(LIB) -o $@
	$(STRIP) $@

install: 
	install -o bin -g bin -m 755 $(EXE) $(COLBINPATH)
	install -m 644 colsum.1 $(COLMANPATH)

distr:
	lha a $(ARCHIV).lzh $(FILES)

distrzoo:
	zoo a $(ARCHIV).zoo $(FILES)

archive:
	tar czvf $(ARCHIV).tgz $(FILES)

unpack:
	tar xzvf $(ARCHIV).tgz

colsum.man: colsum.1
	$(NROFF) $< > $@
