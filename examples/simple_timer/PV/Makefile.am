#############################################################################
# Copyright 1996-2017 Synopsys, Inc.                                        #
#                                                                           #
# This Synopsys software and all associated documentation are proprietary   #
# to Synopsys, Inc. and may only be used pursuant to the terms and          #
# conditions of a written license agreement with Synopsys, Inc.             #
# All other use, reproduction, modification, or distribution of the         #
# Synopsys software or the associated documentation is strictly prohibited. #
#############################################################################
##  examples/simple_timer/PV/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.

CXX_FILES = \
	    main.cpp \
	    top_PV.cpp

H_FILES = \
	  simple_timer_PV.h \
	  simple_timer_PV_TB.h \
	  top_PV.h

check_PROGRAMS = simple_timer
TESTS = $(check_PROGRAMS) 

simple_timer_SOURCES = $(CXX_FILES) $(H_FILES)

AM_CPPFLAGS = $(COMMONINC)

LDADD = $(COMMONLIB)

simple_timer_INSTALLS = $(simple_timer_SOURCES) golden.log

all:
	@echo "To compile and run this example type"
	@echo "   make check"

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) .
	for file in $(simple_timer_INSTALLS); do \
		$(INSTALL_DATA) $(srcdir)/$$file ./$$file; \
	done

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(simple_timer_INSTALLS); do \
		rm -f ./$$file; \
	done

