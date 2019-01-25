############################################################################
# Copyright 1996-2017 Synopsys, Inc.                                        
#                                                                           
# This Synopsys software and all associated documentation are proprietary  
# to Synopsys, Inc. and may only be used pursuant to the terms and         
# conditions of a written license agreement with Synopsys, Inc.             
# All other use, reproduction, modification, or distribution of the         
# Synopsys software or the associated documentation is strictly prohibited. 
############################################################################
noinst_LIBRARIES = libscml2.a

H_FILES = 	abstraction_level_switch_observer.h \
	autosync_callback.h \
	bitfield.h \
	bitfield_access_restriction_b.h \
	bitfield_access_restriction_base.h \
	bitfield_default_restriction.h \
	memory_access_restriction_base.h \
	memory_null_access_restriction.h \
	memory_default_access_restriction.h \
	memory_restriction.h \
	restriction_adapter.h \
	restriction_adapter_tagged.h \
	ignore_restriction_adapter.h \
	error_restriction_adapter.h \
	word_restriction_adapter.h \
	word_restriction_adapter_tagged.h \
	only_word_restriction_adapter.h \
	write_once_restriction_adapter.h \
	memory_restriction_functions_include.h \
	bitfield_restriction.h \
	bitfield_restriction_adapter.h \
	bitfield_restriction_adapter_tagged.h \
	bitfield_restriction_functions_include.h \
	bitfield_container.h \
	bitfield_autosync_read_callback.h \
	bitfield_autosync_write_callback.h \
	bitfield_base.h \
	bitfield_callback_base.h \
	bitfield_callback_functions.h \
	bitfield_callback_functions_include.h \
	bitfield_callback_observer.h \
	bitfield_callback_observer_owner.h \
	bitfield_callback_observers.h \
	bitfield_debug_callback_base.h \
	bitfield_debug_callback_functions.h \
	bitfield_debug_mask_read_adapter.h \
	bitfield_debug_mask_read_adapter_tagged.h \
	bitfield_debug_mask_write_adapter.h \
	bitfield_debug_mask_write_adapter_tagged.h \
	bitfield_debug_read_adapter.h \
	bitfield_debug_read_adapter_tagged.h \
	bitfield_debug_read_callback.h \
	bitfield_debug_read_callback_base.h \
	bitfield_debug_write_adapter.h \
	bitfield_debug_write_adapter_tagged.h \
	bitfield_debug_write_callback.h \
	bitfield_debug_write_callback_base.h \
	bitfield_default_read_behavior_adapter.h \
	bitfield_default_write_behavior_adapter.h \
	bitfield_disallow_debug_read_access_callback.h \
	bitfield_disallow_debug_write_access_callback.h \
	bitfield_disallow_read_access_callback.h \
	bitfield_disallow_write_access_callback.h \
	bitfield_enum_post_write_decoder.h \
	bitfield_mask_read_adapter.h \
	bitfield_mask_read_adapter_tagged.h \
	bitfield_mask_write_adapter.h \
	bitfield_mask_write_adapter_tagged.h \
	bitfield_neversync_read_callback.h \
	bitfield_neversync_write_callback.h \
	bitfield_null_adapter.h \
	bitfield_on_write_adapter.h \
	bitfield_post_write_adapter.h \
	bitfield_post_write_old_new_adapter.h \
	bitfield_post_write_old_new_adapter_tagged.h \
	bitfield_post_write_adapter_tagged.h \
	bitfield_autosync_post_write_callback.h \
	bitfield_default_post_write_callback.h \
	bitfield_neversync_post_write_callback.h \
	bitfield_post_write_callback.h \
	bitfield_post_write_callback_base.h \
	bitfield_selfsync_post_write_callback.h \
	bitfield_read_adapter.h \
	bitfield_read_adapter_tagged.h \
	bitfield_read_callback.h \
	bitfield_read_callback_base.h \
	bitfield_selfsync_read_callback.h \
	bitfield_selfsync_write_callback.h \
	bitfield_set_on_read_adapter.h \
	bitfield_store_after_read_adapter.h \
	bitfield_untimed_mask_read_adapter.h \
	bitfield_untimed_mask_read_adapter_tagged.h \
	bitfield_untimed_mask_write_adapter.h \
	bitfield_untimed_mask_write_adapter_tagged.h \
	bitfield_untimed_read_adapter.h \
	bitfield_untimed_read_adapter_tagged.h \
	bitfield_untimed_write_adapter.h \
	bitfield_untimed_write_adapter_tagged.h \
	bitfield_write_adapter.h \
	bitfield_write_adapter_tagged.h \
	bitfield_write_callback.h \
	bitfield_write_callback_base.h \
	bitfield_write_once_adapter.h \
	burst_unroller.h \
	callback_macro.h \
	callback_observer.h \
	callback_observer_owner.h \
	callback_observers.h \
	set_on_read_adapter.h \
	debug_burst_unroller.h \
	debug_callback.h \
	debug_transport_adapter.h \
	debug_transport_adapter_tagged.h \
	default_behavior_adapter.h \
	dmi_handler.h \
	filter_adapter.h \
	initiator_socket.h \
	severity.h \
	mappable_if.h \
	mapped_range.h \
	mapped_ranges_observer.h \
	mapped_ranges_observer_owner.h \
	mapped_ranges_observers.h \
	memory.h \
	memory_alias.h \
	memory_base.h \
	restriction_owner.h \
	memory_behavior_functions.h \
	memory_callback.h \
	memory_callback_base.h \
	memory_callback_functions.h \
	memory_callback_functions_include.h \
	memory_const_iterator.h \
	memory_content_observer.h \
	memory_content_observer_owner.h \
	memory_content_observers.h \
	memory_debug_callback.h \
	memory_debug_callback_base.h \
	memory_debug_callback_functions.h \
	memory_disallow_access_callback.h \
	memory_disallow_debug_access_callback.h \
	memory_dmi.h \
	memory_fast_access.h \
	memory_fast_access_impl.h \
	memory_index_reference.h \
	memory_iterator.h \
	memory_iterator_base.h \
	memory_region.h \
	memory_region_registry.h \
	memory_select_callback_base.h \
	memory_select_callback.h \
	memory_slow_access_base.h \
	neversync_callback.h \
	null_adapter.h \
	on_write_adapter.h \
	pair_signal.h \
	pin_callback.h \
	pin_callback_base.h \
	pin_callback_functions.h \
	pin_callback_tagged.h \
	post_write_adapter.h \
	post_write_adapter_tagged.h \
	quantumkeeper_if.h \
	quantumkeeper_wrapper.h \
	read_adapter.h \
	read_adapter_tagged.h \
	read_extensions_adapter.h \
	read_extensions_adapter_tagged.h \
	reg.h \
	reg_vector_creator.h \
	resource.h \
	router.h \
	router_base.h \
	router_callback.h \
	router_callback_base.h \
	router_callback_functions.h \
	router_debug_callback.h \
	router_debug_callback_base.h \
	router_debug_callback_functions.h \
	scoped_reference.h \
	vector.h \
	selfsync_callback.h \
	status.h \
	status_observer.h \
	status_observer_owner.h \
	status_registry.h \
	store_after_read_adapter.h \
	stream.h \
	streams.h \
	symbolic_names.h \
	thread_pool.h \
	timing_annotation_registry.h \
	tlm2_gp_extensions.h \
	tlm2_gp_initiator_adapter.h \
	tlm2_gp_initiator_adapter_vector_creator.h \
	tlm2_gp_target_adapter.h \
	tlm2_gp_default_mm.h \
	tlm2_gp_default_mm_creator.h \
	tlm2_gp_mm_dispatcher.h \
	tlm2_gp_mm_ifs.h \
	toplevel_memory_base.h \
	transport_adapter.h \
	transport_adapter_tagged.h \
	trigger_signal.h \
	types.h \
	untimed_read_adapter.h \
	untimed_read_adapter_tagged.h \
	untimed_read_extensions_adapter.h \
	untimed_read_extensions_adapter_tagged.h \
	untimed_transport_adapter.h \
	untimed_transport_adapter_tagged.h \
	untimed_word_read_adapter.h \
	untimed_word_read_adapter_tagged.h \
	untimed_word_read_extensions_adapter.h \
	untimed_word_read_extensions_adapter_tagged.h \
	untimed_word_write_adapter.h \
	untimed_word_write_adapter_tagged.h \
	untimed_word_write_extensions_adapter.h \
	untimed_word_write_extensions_adapter_tagged.h \
	untimed_write_adapter.h \
	untimed_write_adapter_tagged.h \
	untimed_write_extensions_adapter.h \
	untimed_write_extensions_adapter_tagged.h \
	utils.h \
	word_access_unroller.h \
	word_access_unroller_base.h \
	word_adapter_base.h \
	word_read_adapter.h \
	word_read_adapter_tagged.h \
	word_read_extensions_adapter.h \
	word_read_extensions_adapter_tagged.h \
	word_write_adapter.h \
	word_write_adapter_tagged.h \
	word_write_extensions_adapter.h \
	word_write_extensions_adapter_tagged.h \
	write_adapter.h \
	write_adapter_tagged.h \
	write_extensions_adapter.h \
	write_extensions_adapter_tagged.h \
	write_once_adapter.h \
	write_once_state.h \
	clocked_module.h \
	clocked_callback.h \
	clocked_event.h \
	clocked_peq_container_impl.h \
	clocked_peq_container.h \
	clocked_peq.h \
	clocked_timer.h \
	tagged_message.h \
	tagged_message_macros.h \
	memory_put_callback.h \
	memory_put_callback_adapter.h \
	memory_tagged_put_callback_adapter.h \
	bitfield_put_callback.h \
	bitfield_put_callback_adapter.h \
	bitfield_tagged_put_callback_adapter.h \
	memory_put_callback_base.h \
	memory_put_callback_owner.h \
	memory_put_callback_functions_include.h

PRIVATE_H_FILES = 	abstraction_level_switch.h \
	bitfield_default_debug_read_callback.h \
	bitfield_default_debug_write_callback.h \
	bitfield_default_read_callback.h \
	bitfield_default_write_callback.h \
	find_alias.h \
	memory_bitfields_debug_read_callback.h \
	memory_bitfields_debug_write_callback.h \
	memory_bitfields_read_callback.h \
	memory_bitfields_write_callback.h \
	memory_default_callback.h \
	memory_default_debug_callback.h \
	memory_heap_storage.h \
	memory_null_callback.h \
	memory_null_debug_callback.h \
	memory_slow_access.h \
	memory_slow_access_content_observer_decorator.h \
	memory_slow_access_storage_decorator.h \
	memory_storage_base.h \
	memory_storage_creator.h \
	router_null_callback.h \
	router_null_debug_callback.h

noinst_HEADERS = $(H_FILES)

CXX_FILES = 	abstraction_level_switch.cc \
	bitfield.cc \
	bitfield_base.cc \
	bitfield_container.cc \
	bitfield_callback_observers.cc \
	bitfield_disallow_debug_read_access_callback.cc \
	bitfield_disallow_debug_write_access_callback.cc \
	bitfield_disallow_read_access_callback.cc \
	bitfield_disallow_write_access_callback.cc \
	burst_unroller.cc \
	callback_observers.cc \
	debug_burst_unroller.cc \
	dmi_handler.cc \
	mapped_ranges_observers.cc \
	memory.cc \
	memory_alias.cc \
	memory_base.cc \
	restriction_owner.cc \
	memory_bitfields_debug_read_callback.cc \
	memory_bitfields_debug_write_callback.cc \
	memory_bitfields_read_callback.cc \
	memory_bitfields_write_callback.cc \
	memory_content_observers.cc \
	memory_disallow_access_callback.cc \
	memory_disallow_debug_access_callback.cc \
	memory_dmi.cc \
	memory_heap_storage.cc \
	memory_region.cc \
	memory_region_registry.cc \
	memory_storage_base.cc \
	memory_storage_creator.cc \
	reg.cc \
	router.cc \
	router_base.cc \
	status.cc \
	status_registry.cc \
	symbolic_names.cc \
	tagged_message_handler.cc \
	timing_annotation_registry.cc \
	toplevel_memory_base.cc \
	word_access_unroller_base.cc \
	word_adapter_base.cc \
	tlm2_gp_mm_dispatcher.cc \
	clocked_callback.cc \
	clocked_event.cc \
	clocked_peq_container_impl.cc \
	clocked_timer.cc \
	tlm2_gp_target_adapter.cc \
	memory_put_callback_base.cc

libscml2_a_SOURCES = $(H_FILES) $(PRIVATE_H_FILES) $(CXX_FILES)

INCLUDES = -I$(srcdir)/.. $(OSCITLM_INC) $(SYSC_INC)

INCLUDE_DIR = $(prefix)/include
LIB_DIR = $(prefix)/lib-$(TARGET_ARCH)

install-data-local: scml2.h $(noinst_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(INCLUDE_DIR)/scml2
	for file in $(noinst_HEADERS); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(INCLUDE_DIR)/scml2/$$file; \
	done
	$(INSTALL_DATA) $(srcdir)/scml2.h $(INCLUDE_DIR)/scml2.h; \
	$(mkinstalldirs) $(LIB_DIR)
	$(INSTALL_DATA) libscml2.a $(LIB_DIR)/libscml2.a
	$(RANLIB) $(LIB_DIR)/libscml2.a

uninstall-local:
	@$(NORMAL_UNINSTALL)
	for file in $(noinst_HEADERS); do \
	  rm -f $(INCLUDE_DIR)/scml2/$$file; \
	done
	-rmdir $(INCLUDE_DIR)/scml2
	rm -f $(LIB_DIR)/libscml2.a
	-rmdir $(prefix)/lib-$(TARGET_ARCH)

