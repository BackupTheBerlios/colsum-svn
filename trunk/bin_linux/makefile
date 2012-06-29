############################################
#                                          #
#  COLSUM - Column Computation Tool        #
#                                          #
#  Linux makefile                          #
#                                          #
#  GNU Make Build Environment              #
#  based on Eclipse IDE Toolchains         #
#                                          #
#  @copyright (c) 2008 Stefan Habermehl    #
#  @license GNU GPL v3+ (see LICENSE.txt)  #
#                                          #
############################################
#
# @version SVN: $Id$
#
############################################

-include ../makefile.init
-include ../makefile.defs

CC         := gcc
EXECUTABLE := colsum

INCLUDES   := -I../include -I../../cflib/include
LIBS       := -L../../cflib/liblin -lm -lcf
CFLAGS     := -c -g1 -O3 -Wall -pedantic -ansi -MMD -DDEBUG
#CFLAGS    := -v -Wall -mshort -fomit-frame-pointer $(DEBUG)
LFLAGS     := $(LIBS)
#LFLAGS     := -static $(LIBS)

OBJECTS    = colsum.o colmod.o

NROFF      = $(shell which nroff) -man -T$(TERM)
#COLMANPATH = /usr/local/man/man1/
COLMANPATH = /usr/share/man/man1/
COLBINPATH = /usr/local/bin/

ARCHIV     = colsum-0.6.2-L

FILES      = src/colsum.c src/colmod.c include/colsum.h \
             man/colsum.1 man/colsum.man doc/colsum_man.html \
             bin/linux/colsum bin_linux/makefile \
             makefile makefile.init makefile.defs makefile.targets

#### target rules

all:     $(EXECUTABLE) man
man:     ../man/colsum.man ../doc/colsum_man.html
archive: all ../archives/$(ARCHIV).tgz


$(EXECUTABLE): $(OBJECTS)
	$(CC) $+ $(LFLAGS) -o $@
	$(STRIP) $@
	cp $@ ../bin/linux/

%.o: ../src/%.c ../include/colsum.h
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@

install: 
	install -o bin -g bin -m 755 $(EXECUTABLE) $(COLBINPATH)
	install -m 644 ../man/colsum.1 $(COLMANPATH)

clean:
	rm -rf $(OBJECTS) *.d $(EXECUTABLE)

../archives/$(ARCHIV).tgz: $(foreach file,$(FILES),../$(file))
	cd .. && tar -czvf $(subst ../,,$@) $(FILES)

../man/colsum.man: ../man/colsum.1
	$(NROFF) $< > $@

../doc/colsum_man.html: ../man/colsum.man
	man2html -title colsum < $< > $@

-include ../makefile.targets

#### COLSUM - end linux makefile
