############################################################################
# Copyright 1996-2017 Synopsys, Inc.                                        
#                                                                           
# This Synopsys software and all associated documentation are proprietary  
# to Synopsys, Inc. and may only be used pursuant to the terms and         
# conditions of a written license agreement with Synopsys, Inc.             
# All other use, reproduction, modification, or distribution of the         
# Synopsys software or the associated documentation is strictly prohibited. 
############################################################################
noinst_LIBRARIES = libscml2_base.a

H_FILES = 	utils.h \
	callback_macros.h \
	object.h \
	object_registry.h \
	named_base.h \
	atom.h \
	callback.h \
	callback_impl.h \
	callback_observer.h \
	callback_observers_owner.h \
	empty_callback_exception.h \
	setter_owner.h \
	getter_owner.h \
	attribute.h \
	attribute_value.h \
	attribute_constant_initializer.h \
	value_attribute.h \
	const_attribute.h \
	protected_attribute.h \
	protected_value_attribute.h \
	behavior.h \
	snps_vp_dll.h

PRIVATE_H_FILES = 

noinst_HEADERS = $(H_FILES)

CXX_FILES = 	object.cc \
	object_registry.cc \
	callback.cc \
	named_base.cc \
	atom.cc \
	empty_callback_exception.cc

libscml2_base_a_SOURCES = $(H_FILES) $(PRIVATE_H_FILES) $(CXX_FILES)

INCLUDES = -I$(srcdir)/.. $(OSCITLM_INC) $(SYSC_INC)

INCLUDE_DIR = $(prefix)/include
LIB_DIR = $(prefix)/lib-$(TARGET_ARCH)

install-data-local: scml2_base.h $(noinst_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(INCLUDE_DIR)/scml2_base
	for file in $(noinst_HEADERS); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(INCLUDE_DIR)/scml2_base/$$file; \
	done
	$(INSTALL_DATA) $(srcdir)/scml2_base.h $(INCLUDE_DIR)/scml2_base.h; \
	$(mkinstalldirs) $(LIB_DIR)
	$(INSTALL_DATA) libscml2_base.a $(LIB_DIR)/libscml2_base.a
	$(RANLIB) $(LIB_DIR)/libscml2_base.a

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(noinst_HEADERS); do \
	  rm -f $(INCLUDE_DIR)/scml2_base/$$file; \
	done
	-rmdir $(INCLUDE_DIR)/scml2_base
	rm -f $(LIB_DIR)/libscml2_base.a
	-rmdir $(prefix)/lib-$(TARGET_ARCH)

