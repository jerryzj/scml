#############################################################################
# Copyright 1996-2017 Synopsys, Inc.                                        #
#                                                                           #
# This Synopsys software and all associated documentation are proprietary   #
# to Synopsys, Inc. and may only be used pursuant to the terms and          #
# conditions of a written license agreement with Synopsys, Inc.             #
# All other use, reproduction, modification, or distribution of the         #
# Synopsys software or the associated documentation is strictly prohibited. #
#############################################################################
noinst_LIBRARIES = libscml.a

H_FILES = 

PRIVATE_H_FILES =

noinst_HEADERS = $(H_FILES)

CXX_FILES =

libscml_a_SOURCES = $(H_FILES) $(PRIVATE_H_FILES) $(CXX_FILES)

INCLUDES = -I$(srcdir)/.. $(OSCITLM_INC) $(SYSC_INC)

SCMLLIBS = ../scml2/libscml2.a \
    ../scml2_base/libscml2_base.a \
    ../scml2_logging/libscml2_logging.a \
    ../ModelingObjects/libModelingObjects.a \
    ../scml_clock/libscml_clock.a

INCLUDE_DIR = $(prefix)/include
LIB_DIR = $(prefix)/lib-$(TARGET_ARCH)

libscml.a: $(SCMLLIBS)
	@rm -rf $@ *.o
	for file in $(SCMLLIBS) ; do \
	  $(AR) -x $$file ; \
	done;
	$(AR) r $@ *.o
	$(RANLIB) $@
	@rm -rf *.o

install-data-local: libscml.a
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(LIB_DIR)
	$(INSTALL_DATA) libscml.a $(LIB_DIR)/libscml.a

uninstall-local:
	@$(NORMAL_UNINSTALL)
	rm -f $(LIB_DIR)/libscml.a
	-rmdir $(prefix)/lib-$(TARGET_ARCH)
