#############################################################################
# Copyright 1996-2017 Synopsys, Inc.                                        #
#                                                                           #
# This Synopsys software and all associated documentation are proprietary   #
# to Synopsys, Inc. and may only be used pursuant to the terms and          #
# conditions of a written license agreement with Synopsys, Inc.             #
# All other use, reproduction, modification, or distribution of the         #
# Synopsys software or the associated documentation is strictly prohibited. #
#############################################################################
##  examples/HWaccelerator/PV/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.

CXX_FILES = \
	    main.cpp \
	    HWaccelerator_Target_PV.cpp \
	    testInitiator.cpp

H_FILES = \
	  global_defs.h \
	  JPEG_globals.h \
	  HWaccelerator_Target_PV.h \
	  testInitiator.h

check_PROGRAMS = HWaccelerator
TESTS = $(check_PROGRAMS) 

HWaccelerator_SOURCES = $(CXX_FILES) $(H_FILES)

AM_CPPFLAGS = $(COMMONINC)

LDADD = $(COMMONLIB)

HWaccelerator_INSTALLS = $(HWaccelerator_SOURCES) golden.log

all:
	@echo "To compile and run this example type"
	@echo "   make check"

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) .
	for file in $(HWaccelerator_INSTALLS); do \
		$(INSTALL_DATA) $(srcdir)/$$file ./$$file; \
	done

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(HWaccelerator_INSTALLS); do \
		rm -f ./$$file; \
	done

