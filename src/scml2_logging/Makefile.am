#############################################################################
# Copyright 1996-2017 Synopsys, Inc.                                        #
#                                                                           #
# This Synopsys software and all associated documentation are proprietary   #
# to Synopsys, Inc. and may only be used pursuant to the terms and          #
# conditions of a written license agreement with Synopsys, Inc.             #
# All other use, reproduction, modification, or distribution of the         #
# Synopsys software or the associated documentation is strictly prohibited. #
#############################################################################
noinst_LIBRARIES = libscml2_logging.a

H_FILES = \
	and_match.h \
	registry.h \
	exit_logger.h \
	severity.h \
	severity_name_match.h \
	logger_base.h \
	name_match.h \
	name_prefix_match.h \
	or_match.h \
	rule.h \
	rule_base.h \
	severity_threshold_match.h \
	stream.h \
	stream_base.h \
	stream_base_s.h \
	stream_logger.h \
	streambuf.h \
	snps_vp_dll.h

PRIVATE_H_FILES =

noinst_HEADERS = $(H_FILES)

CXX_FILES = \
        registry.cc \
	exit_logger.cc \
	logger_base.cc \
	stream_base.cc \
	stream_base_s.cc \
	stream_logger.cc \
	streambuf.cc


libscml2_logging_a_SOURCES = $(H_FILES) $(PRIVATE_H_FILES) $(CXX_FILES)

INCLUDES = -I$(srcdir)/.. $(OSCITLM_INC) $(SYSC_INC)

INCLUDE_DIR = $(prefix)/include
LIB_DIR = $(prefix)/lib-$(TARGET_ARCH)

.PHONY: scml2_logging.h
scml2_logging.h:
	@rm -rf $@
	@echo "/***************************************************************************" > $@
	@echo " * Copyright 1996-2017 Synopsys, Inc." >> $@
	@echo " *" >> $@
	@echo " * This Synopsys software and all associated documentation are proprietary" >> $@
	@echo " * to Synopsys, Inc. and may only be used pursuant to the terms and" >> $@
	@echo " * conditions of a written license agreement with Synopsys, Inc." >> $@
	@echo " * All other use, reproduction, modification, or distribution of the" >> $@
	@echo " * Synopsys software or the associated documentation is strictly prohibited." >> $@
	@echo " ***************************************************************************/" >> $@
	@echo "" >> $@
	@echo "#ifndef SCML2_LOGGING_H" >> $@
	@echo "#define SCML2_LOGGING_H" >> $@
	@echo "" >> $@
	@for file in $(noinst_HEADERS) ; do \
	  echo "#include \"scml2_logging/$$file\"" >> $@ ; \
	done
	@echo "" >> $@
	@echo "#endif" >> $@

install-data-local: scml2_logging.h
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(INCLUDE_DIR)/scml2_logging
	for file in $(noinst_HEADERS); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(INCLUDE_DIR)/scml2_logging/$$file; \
	done
	$(INSTALL_DATA) scml2_logging.h $(INCLUDE_DIR)/scml2_logging.h; \
	$(mkinstalldirs) $(LIB_DIR)
	$(INSTALL_DATA) libscml2_logging.a $(LIB_DIR)/libscml2_logging.a
	$(RANLIB) $(LIB_DIR)/libscml2_logging.a

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(noinst_HEADERS); do \
	  rm -f $(INCLUDE_DIR)/scml2_logging/$$file; \
	done
	-rmdir $(INCLUDE_DIR)/scml2_logging
	rm -f $(LIB_DIR)/libscml2_logging.a
	-rmdir $(prefix)/lib-$(TARGET_ARCH)
