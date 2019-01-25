/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/toplevel_memory_base.h>

#include <scml2/burst_unroller.h>
#include <scml2/memory_storage_base.h>
#include <scml2/memory_slow_access.h>
#include <scml2/memory_fast_access_impl.h>
#include <scml2/memory_slow_access_content_observer_decorator.h>
#include <scml2/memory_slow_access_storage_decorator.h>
#include <scml2/timing_annotation_registry.h>
#include <scml2/find_alias.h>
#include <scml2/utils.h>

#include <tlm.h>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <memory>

namespace scml2 {

bool 
toplevel_memory_base::
s_old_style_dmi_latency = (::getenv("SNPS_SLS_SCML2_NEW_STYLE_DMI_LATENCY") == 0);

bool 
toplevel_memory_base::
get_old_style_dmi_latency()
{
  return s_old_style_dmi_latency;
}

void 
toplevel_memory_base::
set_old_style_dmi_latency(bool old_style)
{
  if (!s_old_style_dmi_latency && old_style) {
    std::cerr << "WARNING: old style dmi latency has been re-enabled. This might impact timing results of the simulation when non-checking peripherals are in use." << std::endl;
  }
  s_old_style_dmi_latency = old_style;
}

toplevel_memory_base::
toplevel_memory_base(const std::string& name,
                     unsigned long long size,
                     unsigned int width) :
  memory_base(name, 0, 0ull, size, width),
  burst_unroller(width),
  debug_burst_unroller(width),
  mTheStreams(new streams(get_name())),
  mStorage(0),
  mReadLatency(sc_core::SC_ZERO_TIME),
  mWriteLatency(sc_core::SC_ZERO_TIME),
  mReadDmiData(size
	       ),
  mWriteDmiData(size
		),
  mContentObservers(size),
  mDmiPointersInUse(false),
  mDelStorageOnDestruction(false),
  mAlreadyFiltered(false)
{
  init_streams(*mTheStreams);
}

toplevel_memory_base::
~toplevel_memory_base()
{
  if (mDelStorageOnDestruction) {
    delete mStorage;
  }
  delete mTheStreams;
}

void
toplevel_memory_base::
set_storage(memory_storage_base* storage, bool deleteInDestructor)
{
  assert(!mStorage);
  mStorage = storage;
  mDelStorageOnDestruction = deleteInDestructor;
}

void
toplevel_memory_base::
register_bw_direct_mem_if(tlm::tlm_bw_direct_mem_if* bwInterface)
{
  assert(bwInterface);
  mBwDirectMemIfs.insert(bwInterface);
}

void
toplevel_memory_base::
unregister_bw_direct_mem_if(tlm::tlm_bw_direct_mem_if* bwInterface)
{
  mBwDirectMemIfs.erase(bwInterface);
}

void
toplevel_memory_base::
invalidate_dmi()
{
  if (mDmiPointersInUse) {
    std::set<tlm::tlm_bw_direct_mem_if*>::iterator it = mBwDirectMemIfs.begin();
    std::set<tlm::tlm_bw_direct_mem_if*>::const_iterator end = mBwDirectMemIfs.end();
    for (; it != end; ++it) {
      (*it)->invalidate_direct_mem_ptr(0, index_to_address(get_size(), get_width()) - 1);
    }
    mDmiPointersInUse = false;
  }
}

std::string
toplevel_memory_base::
get_mapped_name() const
{
  return get_name();
}

void
toplevel_memory_base::
process_unrolled_transaction(tlm::tlm_generic_payload& trans,
                             sc_core::sc_time& t)
{
  b_transport(trans, t);
}

unsigned int
toplevel_memory_base::
process_unrolled_debug_transaction(tlm::tlm_generic_payload& trans)
{
  return transport_dbg(trans);
}

unsigned int 
toplevel_memory_base::
get_access_size_at_address(unsigned long long address) const
{
  const unsigned long long index = address_to_index(address, get_width());
  if (const memory_base* alias = find_alias(*this, index, index)) {
    return alias->get_width();
  } else {
    // out-of-bounds access, will be handled later
    return get_width();
  }
}

unsigned int 
toplevel_memory_base::
get_debug_access_size_at_address(unsigned long long address) const
{
  const unsigned long long index = address_to_index(address, get_width());
  if (const memory_base* alias = find_alias(*this, index, index)) {
    return alias->get_width();
  } else {
    // out-of-bounds access, will be handled later
    return get_width();    
  }
}

class toplevel_memory_base::filter_helper
  : public burst_unroller
{
public:
  filter_helper(toplevel_memory_base& mem, tlm::tlm_generic_payload& trans)
    : burst_unroller(mem.get_width())
    , m_mem(mem)
    , m_already_filtered(m_mem.already_filtered())
    , m_be_helper(trans)
    , m_data_helper(trans)
    , m_filter_error(false)
  {
    if (!m_already_filtered) {
      if (trans.is_write()) {
	m_data.insert(m_data.end(), &trans.get_data_ptr()[0], &trans.get_data_ptr()[trans.get_data_length()]);
	m_data_helper.engage(&m_data[0]);
      }
      sc_core::sc_time t;
      unroll_transaction(trans, t);
      m_filter_error = (trans.get_response_status() == tlm::TLM_GENERIC_ERROR_RESPONSE);
      if (!m_filter_error) {
	adapt_byte_enables_from_unrolled_trans(trans);
	assert(trans.get_data_length() == m_filtered_byte_enables.size());
	m_be_helper.engage(&m_filtered_byte_enables[0]);
      }
    }
    m_mem.set_already_filtered(true);
  }
  
  ~filter_helper()
  {
    m_mem.set_already_filtered(m_already_filtered);
  }

  bool get_filter_error() const 
  {
    return m_filter_error;
  }
  
private:
  void adapt_byte_enables_from_unrolled_trans(tlm::tlm_generic_payload& trans)
  {
    unsigned width = m_mem.get_width();
    unsigned eff_sw = std::min(trans.get_data_length(), trans.get_streaming_width());
    unsigned front_gap = trans.get_address()%get_access_size_at_address(trans.get_address());
    unsigned long long last_aligned_addr = ((trans.get_address() + eff_sw -1)/width)*width;
    unsigned back_gap = (unsigned)(last_aligned_addr + get_access_size_at_address(last_aligned_addr) - trans.get_address() - eff_sw);
    unsigned data_left = trans.get_data_length();
    std::vector<unsigned char>::iterator i = m_filtered_byte_enables.begin();
    while (data_left != 0) {
      assert(m_filtered_byte_enables.end() >= i + front_gap);
      m_filtered_byte_enables.erase(i, i + front_gap);
      unsigned beat_sw = std::min(data_left, eff_sw);
      data_left -= beat_sw;
      assert(m_filtered_byte_enables.end() >= i + beat_sw);
      i+=beat_sw;
      if (data_left != 0) {
	assert(m_filtered_byte_enables.end() >= i + back_gap);
	m_filtered_byte_enables.erase(i, i + back_gap);	
      } else {
	m_filtered_byte_enables.erase(i, m_filtered_byte_enables.end());	
      }
    }
  }

  virtual void process_unrolled_transaction(tlm::tlm_generic_payload& trans, sc_core::sc_time& /*t*/)
  {
    if (trans.get_byte_enable_ptr() != 0) {
      for (unsigned i = 0; i != trans.get_data_length(); ++i) {
	m_filtered_byte_enables.push_back(trans.get_byte_enable_ptr()[i % trans.get_byte_enable_length()]);
      }
    } else {
      m_filtered_byte_enables.insert(m_filtered_byte_enables.end(),trans.get_data_length(), 0xff);
    }
    trans_byte_enable_helper helper(trans, &m_filtered_byte_enables[m_filtered_byte_enables.size()-trans.get_data_length()]);
    m_mem.filter(trans);
  }
  virtual unsigned int get_access_size_at_address(unsigned long long address) const
  {
    return m_mem.get_access_size_at_address(address);
  }
private:
  toplevel_memory_base& m_mem;
  std::vector<unsigned char> m_data;
  std::vector<unsigned char> m_filtered_byte_enables;
  bool m_already_filtered;
  trans_byte_enable_helper m_be_helper;
  trans_data_helper m_data_helper;
  bool m_filter_error;
};

namespace {
  bool should_apply_restrictions(memory_base* alias, bool readNotWrite) {
    if (alias == 0) {
      return false; // no restriction anywhere in toplevel or out-of-range access
    }
    // is there any restriction below? => run restrictions
    if ((readNotWrite && alias->get_partial_read_restriction_count() != 0) || 
	(!readNotWrite && alias->get_partial_write_restriction_count() != 0)) {
      return true;
    }
    // does any parent have a restriction? => run restrictions
    while (alias != 0) {
      if ((readNotWrite && !alias->get_read_restriction()->has_default_restriction()) || 
	  (!readNotWrite && !alias->get_write_restriction()->has_default_restriction())) {
	return true;
      }
      alias = alias->get_parent();
    }
    // if not, there are no restrictions for this access
    return false;
  }
}

void
toplevel_memory_base::
b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
  const unsigned long long address = trans.get_address();
  const unsigned long long lowIndex = address_to_index(address, get_width());
  const unsigned long long highIndex =
    address_to_index(
      address + std::min(trans.get_data_length(), trans.get_streaming_width()) - 1,
      get_width());

  memory_base* alias = 0;
  bool apply_restrictions = false;
  if (trans.is_read()) {
    alias = find_alias(*this, lowIndex, highIndex, HasNonDefaultReadBehavior());
    apply_restrictions = should_apply_restrictions(get_partial_read_restriction_count() != 0 ? find_alias(*this, lowIndex, highIndex) : 0, true);
  } else if (trans.is_write()) {
    alias = find_alias(*this, lowIndex, highIndex, HasNonDefaultWriteBehavior());
    apply_restrictions = should_apply_restrictions(get_partial_write_restriction_count() != 0 ? find_alias(*this, lowIndex, highIndex) : 0, false);
  } else {
    assert(trans.get_command() == tlm::TLM_IGNORE_COMMAND);
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    return;
  }

  std::unique_ptr<filter_helper> fil_helper;
  if (apply_restrictions) {
    fil_helper.reset(new filter_helper(*this, trans));
    if (fil_helper->get_filter_error()) {
      return;
    }
  }

  if (!alias) {
    if (lowIndex != highIndex) {
      unroll_transaction(trans, t);
    } else {
      SCML2_ERROR(ACCESS_UNMAPPED_ADDRESS) << "Access to address 0x" << std::hex << address << " is not allowed." << std::endl;
      trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
    }
  } else {
    if (are_bytes_enabled(trans, alias->get_size()*alias->get_width())) {
      trans_address_helper address_helper(trans, index_to_address(alias->get_offset(), get_width()));
      if ((!alias->has_default_read_behavior() && trans.is_read()) ||
	  (!alias->has_default_write_behavior() && trans.is_write()) ||
	  mContentObservers.has_observers() ||
	  mCallbackObservers.has_observers()) {
	timing_annotation_registry::get_instance().register_timing_annotation(t);
	alias->execute_behavior(trans, t);
	timing_annotation_registry::get_instance().unregister_timing_annotation();
      } else {
	alias->execute_behavior(trans, t);
      }
    } else {
      trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }
  }

  trans.set_dmi_allowed(true);
}

void
toplevel_memory_base::
addReadDmiData(memory_base* alias)
{
  assert(alias->get_parent());
  if (alias->is_dmi_read_allowed()) {
    AliasesConstIterator it = alias->begin_alias();
    AliasesConstIterator end = alias->end_alias();
    for (; it != end; ++it) {
      addReadDmiData(*it);
    }

  } else {
    mReadDmiData.disable(alias->get_offset(),
                         alias->get_offset() + alias->get_size() - 1);
  }
}

void
toplevel_memory_base::
addWriteDmiData(memory_base* alias)
{
  assert(alias->get_parent());
  if (alias->is_dmi_write_allowed()) {
    AliasesConstIterator it = alias->begin_alias();
    AliasesConstIterator end = alias->end_alias();
    for (; it != end; ++it) {
      addWriteDmiData(*it);
    }

  } else {
    mWriteDmiData.disable(alias->get_offset(),
                          alias->get_offset() + alias->get_size() - 1);
  }
}

void
toplevel_memory_base::
addDmiData(memory_base* alias)
{
  if (alias->is_dmi_allowed()) {
    AliasesConstIterator it = alias->begin_alias();
    AliasesConstIterator end = alias->end_alias();

    for (; it != end; ++it) {
      addDmiData(*it);
    }

  } else if (alias->is_dmi_read_allowed()) {
    mWriteDmiData.disable(alias->get_offset(),
                          alias->get_offset() + alias->get_size() - 1);

    AliasesConstIterator it = alias->begin_alias();
    AliasesConstIterator end = alias->end_alias();
    for (; it != end; ++it) {
      addReadDmiData(*it);
    }

  } else if (alias->is_dmi_write_allowed()) {
    mReadDmiData.disable(alias->get_offset(),
                         alias->get_offset() + alias->get_size() - 1);

    AliasesConstIterator it = alias->begin_alias();
    AliasesConstIterator end = alias->end_alias();
    for (; it != end; ++it) {
      addWriteDmiData(*it);
    }

  } else {
    unsigned long long startIndex = alias->get_offset();
    unsigned long long endIndex = alias->get_offset() + alias->get_size() - 1;
    mReadDmiData.disable(startIndex, endIndex);
    mWriteDmiData.disable(startIndex, endIndex);
  }
}

bool
toplevel_memory_base::
get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data)
{
  const unsigned long long address = trans.get_address();

  if (!mDmiPointersInUse) {

    if (is_dmi_enabled()) {
      mReadDmiData.clear();
      mWriteDmiData.clear();
      addDmiData(this);
      mContentObservers.disable_dmi(mReadDmiData, mWriteDmiData);
    } else {
      mReadDmiData.disable();
      mWriteDmiData.disable();
    }

    mDmiPointersInUse = true;
  }

  assert(trans.get_command() != tlm::TLM_IGNORE_COMMAND);

  const unsigned long long index = address_to_index(address, get_width());
  const memory_dmi::DmiDataEntry& dmiData =
   trans.is_read() ? mReadDmiData.find(index) :
                     mWriteDmiData.find(index);

  dmi_data.set_start_address(index_to_address(dmiData.startIndex, get_width()));
  dmi_data.set_end_address(index_to_address(dmiData.endIndex, get_width()) + get_width() - 1);

  if (trans.is_read()) {
    dmi_data.allow_read();
    dmi_data.set_read_latency(get_default_read_latency() / (get_old_style_dmi_latency() ? 1 : get_size()));
  } else {
    dmi_data.allow_write();
    dmi_data.set_write_latency(get_default_write_latency() / (get_old_style_dmi_latency() ? 1 : get_size()));
  }

  if (dmiData.dmiAllowed) {
    assert(mStorage);
    unsigned char* fastBackdoor = mStorage->get_fast_backdoor();
    if (fastBackdoor) {
      dmi_data.set_dmi_ptr(fastBackdoor + dmi_data.get_start_address());
      return true;

    } else {
      memory_storage_base::fast_backdoor_region region =
        mStorage->get_fast_backdoor(address);
      if (region.fastBackdoor) {
        if (region.startAddress > dmi_data.get_start_address()) {
          dmi_data.set_start_address(region.startAddress);
        }
        if (region.endAddress < dmi_data.get_end_address()) {
          dmi_data.set_end_address(region.endAddress);
        }
        dmi_data.set_dmi_ptr(region.fastBackdoor -
                             region.startAddress +
                             dmi_data.get_start_address());
        return true;
      }
    }
  }

  return false;
}

unsigned int
toplevel_memory_base::
transport_dbg(tlm::tlm_generic_payload& trans)
{
  const unsigned long long address = trans.get_address();
  const unsigned long long lowIndex = address_to_index(address, get_width());
  const unsigned long long highIndex =
    address_to_index(address + trans.get_data_length() - 1, get_width());

  memory_base* alias = 0;
  if (trans.is_read()) {
    alias = find_alias(*this, lowIndex, highIndex, HasNonDefaultDebugReadBehavior());

  } else if (trans.is_write()) {
    alias = find_alias(*this, lowIndex, highIndex, HasNonDefaultDebugWriteBehavior());

  } else {
    assert(trans.get_command() == tlm::TLM_IGNORE_COMMAND);
    return 0;
  }

  trans.set_dmi_allowed(true);

  if (!alias) {
    if (lowIndex != highIndex) {
      return unroll_debug_transaction(trans);

    } else {
      return 0;
    }

  } else {
    trans.set_address(address - index_to_address(alias->get_offset(), get_width()));
    unsigned int result = alias->execute_debug_behavior(trans);
    trans.set_address(address);
    return result;
  }
}

unsigned int
toplevel_memory_base::
read(unsigned long long address,
     unsigned char* data,
     unsigned int size)
{
  size = (unsigned int)std::min<unsigned long long>(size,
						    index_to_address(get_size(),
								     get_width()));
  return get_debug_with_triggering_callbacks(address, data, size);
}

unsigned int
toplevel_memory_base::
write(unsigned long long address,
      const unsigned char* data,
      unsigned int size)
{
  size = (unsigned int)std::min<unsigned long long>(size,
						    index_to_address(get_size(),
								     get_width()));
  return put_debug_with_triggering_callbacks(address, data, size);
}

bool
toplevel_memory_base::
register_content_observer(memory_content_observer* o)
{
  return register_content_observer(o, 0);
}

bool
toplevel_memory_base::
unregister_all_content_observers()
{
  if (mContentObservers.has_observers()) {
    if (mContentObservers.unregister_all_observers()) {
      invalidate_dmi();
      propagate_update_fast_access();
      return true;

    } else {
      return false;
    }
  }

  return true;
}

bool
toplevel_memory_base::
unregister_content_observer(memory_content_observer* o)
{
  bool oldHasObservers = mContentObservers.has_observers();

  if (mContentObservers.unregister_observer(o)) {
    invalidate_dmi();
    if (oldHasObservers != mContentObservers.has_observers()) {
      propagate_update_fast_access();
    }
    return true;
  }

  return false;
}


bool
toplevel_memory_base::
register_content_observer(memory_content_observer* o,
                          unsigned long long offset)
{
  bool oldHasObservers = mContentObservers.has_observers();

  if (mContentObservers.register_observer(o, offset)) {
    invalidate_dmi();
    if (oldHasObservers != mContentObservers.has_observers()) {
      propagate_update_fast_access();
    }
    return true;
  }

  return false;
}

void
toplevel_memory_base::
fast_backdoor_changed()
{
  invalidate_dmi();
  propagate_update_fast_access();
}


const char*
toplevel_memory_base::
kind() const
{
  return "scml2::memory";
}

template <typename DT>
memory_slow_access_base<DT>*
toplevel_memory_base::
create_slow_access(unsigned long long offset)
{
  assert(mStorage);
  unsigned char* fastBackdoor = mStorage->get_fast_backdoor();
  if (fastBackdoor) {
    // backdoor pointer available -> fast inline code
    if (mContentObservers.has_observers()) {
	return ::scml2::create_slow_access<DT>
	  (create_slow_access_content_observer_decorator<DT>
	   (memory_fast_access_impl<DT>(fastBackdoor + get_width()*offset),
	    mContentObservers,
	    offset, get_width()));
    } else {
	return ::scml2::create_slow_access<DT>
	  (memory_fast_access_impl<DT>(fastBackdoor + get_width()*offset));
    }

  } else {
    // no backdoor pointer available -> use virtual read/write methods of storage class
    if (mContentObservers.has_observers()) {
	return ::scml2::create_slow_access<DT>
	  (create_slow_access_content_observer_decorator<DT>
	   (memory_slow_access_storage_decorator<DT>(*mStorage, (offset*get_width())/sizeOf<DT>()),
	    mContentObservers,
	    offset, get_width()));
    } else {
	return ::scml2::create_slow_access<DT>
	  (memory_slow_access_storage_decorator<DT>(*mStorage, (offset*get_width())/sizeOf<DT>()));
    }
  }

  SCML2_LOG_ERROR(this, get_streams().internal_error(), UNDEFINED_ERROR)
    << "can not create slow access class" << std::endl;
  return 0;
}

#ifdef _WIN32
#define SCML2_INSTANTIATE_TEMPLATE(type) template SNPS_VP_API memory_slow_access_base<type >* toplevel_memory_base::create_slow_access<type >(unsigned long long);
#else
#define SCML2_INSTANTIATE_TEMPLATE(type) template memory_slow_access_base<type >* toplevel_memory_base::create_slow_access<type >(unsigned long long);
#endif  
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}
