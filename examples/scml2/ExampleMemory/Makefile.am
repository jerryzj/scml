#############################################################################
# Copyright 1996-2017 Synopsys, Inc.                                        #
#                                                                           #
# This Synopsys software and all associated documentation are proprietary   #
# to Synopsys, Inc. and may only be used pursuant to the terms and          #
# conditions of a written license agreement with Synopsys, Inc.             #
# All other use, reproduction, modification, or distribution of the         #
# Synopsys software or the associated documentation is strictly prohibited. #
#############################################################################
##  examples/scml2/ExampleMemory/Makefile.am --
##  Process this file with automake to produce a Makefile.in file.

CXX_FILES = main.cpp

H_FILES = \
	ExampleMemory.h \
	MemoryCheck.h

check_PROGRAMS = main
TESTS = $(check_PROGRAMS) 

## INCLUDES = -I$(prefix)/include

main_SOURCES = $(CXX_FILES) $(H_FILES)

main_CPPFLAGS = $(COMMONINC)

main_LDADD = $(COMMONLIB)

main_INSTALLS = $(main_SOURCES) golden.log

all:
	@echo "To compile and run this example type"
	@echo "   make check"

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) .
	for file in $(main_INSTALLS); do \
		$(INSTALL_DATA) $(srcdir)/$$file ./$$file; \
	done

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(main_INSTALLS); do \
		rm -f ./$$file; \
	done



