#############################################################################
# Copyright 1996-2017 Synopsys, Inc.                                        #
#                                                                           #
# This Synopsys software and all associated documentation are proprietary   #
# to Synopsys, Inc. and may only be used pursuant to the terms and          #
# conditions of a written license agreement with Synopsys, Inc.             #
# All other use, reproduction, modification, or distribution of the         #
# Synopsys software or the associated documentation is strictly prohibited. #
#############################################################################
##  Makefile.am --
##  Process this file with automake to produce a Makefile.in file.
##

H_FILES = \
        PVCustomReq.h \
        PVCustomResp.h \
        PVDataTypes.h \
	PVProtocolReq.h \
	PVProtocolResp.h \
        PV.h \
        tlm_transport_if.h

NO_H_FILES =

noinst_HEADERS = $(H_FILES)

CXX_FILES =

NO_CXX_FILES =

INCLUDE_DIR = $(prefix)/include
PVINCLUDE_DIR = $(INCLUDE_DIR)/PV

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(INCLUDE_DIR)
	$(mkinstalldirs) $(PVINCLUDE_DIR)
	for file in $(noinst_HEADERS); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(PVINCLUDE_DIR)/$$file; \
	done

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(noinst_HEADERS); do \
	  rm -f $(PVINCLUDE_DIR)/$$file; \
	done
	-rmdir $(PVINCLUDE_DIR)

