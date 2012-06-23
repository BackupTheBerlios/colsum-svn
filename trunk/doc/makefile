############################################
#                                          #
#  COLSUM - Column Computation Tool        #
#                                          #
#  makefile for Documentation              #
#                                          #
#  @copyright (c) 2009 Stefan Habermehl    #
#  @license GNU GPL v3+ (see LICENSE.txt)  #
#                                          #
############################################
#
# @version SVN: $Id$
#
############################################

-include ../makefile.init
-include ../makefile.defs

#### Documentation

all: colsum.chm colsum.pdf colsum_html.tgz ..$(DIRSEP)man$(DIRSEP)colsum.1

COLSUM_DOCU_HTML := $(foreach ext, html css gif png, html$(DIRSEP)*.$(ext))

colsum_html.tgz: html$(DIRSEP)index.html $(COLSUM_DOCU_HTML)
	@echo "Creating HTML documentation archive $@"
	tar czvf $@ $(COLSUM_DOCU_HTML)
	@echo 'Finished building target: $@'
	@echo ' '

colsum.pdf: latex$(DIRSEP)refman.pdf
	@echo "Created PDF documentation file $@"
	$(CP) $< $@
	@echo ' '

%$(DIRSEP)refman.pdf: %$(DIRSEP)refman.tex
	@echo "Creating LATeX documentation file $<"
	@cd "$(dir $<)" && make -C . all

..$(DIRSEP)man$(DIRSEP)colsum.1: ..$(DIRSEP)man$(DIRSEP)man1$(DIRSEP)colsum.1
	@echo "Updating Unix manual page $(notdir $@)"
	$(CP) $< $@
	-$(RM) $(dir $<)$(DIRSEP)*
	-$(CP) $@ $<
	@echo ' '

colsum.chm: html$(DIRSEP)index.hhc
	@echo "Created Compiled HTML Help file $@"
	@echo ' '

html$(DIRSEP)index.hhc: html$(DIRSEP)index.html
	@echo "Created HTML Help source file $@"

latex$(DIRSEP)refman.tex html$(DIRSEP)index.html ..$(DIRSEP)man$(DIRSEP)man1$(DIRSEP)colsum.1: ..$(DIRSEP)colsum.doxyfile
	@echo "Calling Doxygen with configuration from $(notdir $<)"
	cd $(dir $<) && doxygen $(notdir $<)
	@echo ' '


# Other Targets
clean:
	-$(RM) html$(DIRSEP)index.h*
	-@echo ' '

.PHONY: all clean

-include ../makefile.targets

############### End of Documentation Makefile
