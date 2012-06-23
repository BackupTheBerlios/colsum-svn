############################################
#                                          #
#  COLSUM - Column Computation Tool        #
#                                          #
#  Root makefile                           #
#                                          #
#  GNU Make Build Environment              #
#  based on Eclipse IDE Toolchains         #
#                                          #
#  @copyright (c) 2008-9 Stefan Habermehl  #
#  @license GNU GPL v3+ (see LICENSE.txt)  #
#                                          #
############################################
#
# @version SVN: $Id: makefile 41 2009-01-31 14:49:24Z stefan $
#
############################################

-include makefile.init

COLSUM_ROOT_REL := .

COLSUM_TARGETS := all clean install man archive unpack

$(COLSUM_TARGETS): %:
	cd $(COLSUM_PROJECT) && $(MAKE) -C . $@

-include makefile.targets

################ End of COLSUM root makefile
