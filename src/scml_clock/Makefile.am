#############################################################################
# Copyright 1996-2017 Synopsys, Inc.                                        #
#                                                                           #
# This Synopsys software and all associated documentation are proprietary   #
# to Synopsys, Inc. and may only be used pursuant to the terms and          #
# conditions of a written license agreement with Synopsys, Inc.             #
# All other use, reproduction, modification, or distribution of the         #
# Synopsys software or the associated documentation is strictly prohibited. #
#############################################################################
noinst_LIBRARIES = libscml_clock.a

H_FILES = \
	scml_clock_if.h \
	engine.h \
	clocked_module.h \
	tick_callback_engine.h \
	scml_clock.h \
	gate.h

PRIVATE_H_FILES = \
	configurator.h \
	divider_configurator.h \
	master_configurator.h

noinst_HEADERS = $(H_FILES)

CXX_FILES = \
	configurator.cc \
	engine.cc \
	tick_callback_engine.cc \
	clocked_module.cc \
	master_configurator.cc \
	divider_configurator.cc \
	scml_clock.cc \
	gate.cc


libscml_clock_a_SOURCES = $(H_FILES) $(PRIVATE_H_FILES) $(CXX_FILES)

INCLUDES = -I$(srcdir)/.. $(SYSC_INC)

INCLUDE_DIR = $(prefix)/include
LIB_DIR = $(prefix)/lib-$(TARGET_ARCH)

install-data-local:
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(INCLUDE_DIR)/scml_clock
	for file in $(noinst_HEADERS); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(INCLUDE_DIR)/scml_clock/$$file; \
	done
	$(mkinstalldirs) $(LIB_DIR)
	$(INSTALL_DATA) libscml_clock.a $(LIB_DIR)/libscml_clock.a
	$(RANLIB) $(LIB_DIR)/libscml_clock.a

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(noinst_HEADERS); do \
	  rm -f $(INCLUDE_DIR)/scml_clock/$$file; \
	done
	-rmdir $(INCLUDE_DIR)/scml_clock
	rm -f $(LIB_DIR)/libscml_clock.a
	-rmdir $(prefix)/lib-$(TARGET_ARCH)
