#############################################################################
# Copyright 1996-2017 Synopsys, Inc.                                        #
#                                                                           #
# This Synopsys software and all associated documentation are proprietary   #
# to Synopsys, Inc. and may only be used pursuant to the terms and          #
# conditions of a written license agreement with Synopsys, Inc.             #
# All other use, reproduction, modification, or distribution of the         #
# Synopsys software or the associated documentation is strictly prohibited. #
#############################################################################
##  examples/cache/PV/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.

CXX_FILES = \
	    main.cpp

H_FILES = \
	  cache.h \
	  initiator.h \
	  target.h \
	  linkedList.h

check_PROGRAMS = cache
TESTS = $(check_PROGRAMS) 

cache_SOURCES = $(CXX_FILES) $(H_FILES)

AM_CPPFLAGS = $(COMMONINC)

LDADD = $(COMMONLIB)

cache_INSTALLS = $(cache_SOURCES) golden.log

all:
	@echo "To compile and run this example type"
	@echo "   make check"

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) .
	for file in $(cache_INSTALLS); do \
		$(INSTALL_DATA) $(srcdir)/$$file ./$$file; \
	done



uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(cache_INSTALLS); do \
		rm -f ./$$file; \
	done

