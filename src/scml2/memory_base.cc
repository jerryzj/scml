/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/memory_base.h>
#include <scml2/memory_null_callback.h>
#include <scml2/memory_null_debug_callback.h>
#include <scml2/memory_callback_base.h>
#include <scml2/memory_debug_callback_base.h>
#include <scml2/memory_null_access_restriction.h>
#include <scml2/streams.h>
#include <scml2/find_alias.h>

#include <scml2_logging/stream.h>
#include <cassert>
#include <sstream>

namespace scml2 {

memory_base::
memory_base(const std::string& name,
            memory_base* parent,
            unsigned long long offset,
            unsigned long long size,
            unsigned int width) :
  memory_region(name, offset, size, width),
  mParent(parent),
  mDefaultCallback(0),
  mDefaultDebugCallback(0),
  mDefaultRestriction(0),
  mReadRestriction(0),
  mWriteRestriction(0),
  mDmiEnabled(true)
{
  mReadCallback = new memory_null_callback(*this);
  mReadCallback->ref();
  mWriteCallback = mReadCallback;
  mWriteCallback->ref();

  mDebugReadCallback = new memory_null_debug_callback(*this);
  mDebugReadCallback->ref();
  mDebugWriteCallback = mDebugReadCallback;
  mDebugWriteCallback->ref();

  mReadRestriction = new memory_null_access_restriction(*this);
  mReadRestriction->ref();
  mWriteRestriction = mReadRestriction;
  mWriteRestriction->ref();

  if (mParent) {
    mParent->register_alias(*this);
  }
}

memory_base::
~memory_base()
{
  mDefaultCallback->unref();
  mDefaultDebugCallback->unref();
  mReadCallback->unref();
  mWriteCallback->unref();
  mDebugReadCallback->unref();
  mDebugWriteCallback->unref();
  mDefaultRestriction->unref();
  mReadRestriction->unref();
  mWriteRestriction->unref();
}

void
memory_base::
propagate_update_fast_access()
{
  update_fast_access();
  AliasesConstIterator it = mAliases.begin();
  AliasesConstIterator end = mAliases.end();
  for (; it != end; ++it) {
    (*it)->propagate_update_fast_access();
  }
}

void
memory_base::
init_default_callback(memory_callback_base* cb)
{
  assert(!mDefaultCallback);
  assert(cb);
  mDefaultCallback = cb;
  mDefaultCallback->ref();
  do_set_read_callback(cb);
  do_set_write_callback(cb);
}

void
memory_base::
init_default_debug_callback(memory_debug_callback_base* cb)
{
  assert(!mDefaultDebugCallback);
  assert(cb);
  mDefaultDebugCallback = cb;
  mDefaultDebugCallback->ref();
  do_set_debug_read_callback(cb);
  do_set_debug_write_callback(cb);
}

void
memory_base::
init_default_restriction(memory_access_restriction_base* r)
{
  assert(!mDefaultRestriction);
  assert(r);
  mDefaultRestriction = r;
  mDefaultRestriction->ref();
  do_set_read_restriction(r);
  do_set_write_restriction(r);
}


const memory_base*
memory_base::get_toplevel() const
{
  if (mParent) {
    return mParent->get_toplevel();
  } else {
    return this;
  }
}

namespace {
  struct alias_comparator {
    bool operator()(const memory_base* lhs, const memory_base* rhs) const {
      return lhs->get_offset() < rhs->get_offset();
    }
  };
}

void
memory_base::
register_alias(memory_base& alias)
{
  if (alias.get_width() % get_width()) {
    SCML2_LOG_ERROR(this, get_streams().error(), CONFIGURATION_ERROR)
      << "The alias '" << alias.get_name()
      << "' has a width (" << alias.get_width() << ") which is not a multiple of its parent '"
      << get_name() << "' width (" << get_width() << ")"
      << std::endl;    
  }

  unsigned int toplevel_width = get_toplevel()->get_width();
  if ((alias.get_offset()*toplevel_width + alias.get_size()*alias.get_width()) > (get_offset()*toplevel_width + get_size()*get_width())) {
    SCML2_LOG_ERROR(this, get_streams().error(), CONFIGURATION_ERROR)
      << "The alias '" << alias.get_name()
      << "' addresses memory that is not in the parent '"
      << get_name() << "'"
      << std::endl;
  }

  // ? keep track of non-overlapping-ness to be able to optimize this check?
  // will add utility function to return range of alias iterators refactored from find_alias
  AliasesConstIterator it;
  AliasesConstIterator end;
  std::tie(it, end) = get_alias_range(*this, alias.get_offset(), alias.get_offset()+alias.get_size());
  for (; it != end; ++it) {
    if ((*it)->get_offset()*toplevel_width < (alias.get_offset()*toplevel_width + alias.get_size()*alias.get_width()) &&
	alias.get_offset()*toplevel_width < ((*it)->get_offset()*toplevel_width + (*it)->get_size()*(*it)->get_width())
	) {
      SCML2_LOG_ERROR(this, get_streams().error(), CONFIGURATION_ERROR)
	<< "The alias '" << alias.get_name()
	<< "' overlaps with alias '" << (*it)->get_name()  << "'. Subsequent overlaps in the same memory might not be reported on correctly."
	<< std::endl;
    }
  }

  mAliases.insert(std::upper_bound(mAliases.begin(), mAliases.end(), &alias, alias_comparator()), &alias);
}

void
memory_base::
do_set_read_callback(memory_callback_base* cb)
{
  assert(mReadCallback);
  assert(cb);
  mReadCallback->unref();
  mReadCallback = cb;
  mReadCallback->ref();
}

void
memory_base::
do_set_write_callback(memory_callback_base* cb)
{
  assert(mWriteCallback);
  assert(cb);
  mWriteCallback->unref();
  mWriteCallback = cb;
  mWriteCallback->ref();
}

void
memory_base::
do_set_debug_read_callback(memory_debug_callback_base* cb)
{
  assert(mDebugReadCallback);
  assert(cb);
  mDebugReadCallback->unref();
  mDebugReadCallback = cb;
  mDebugReadCallback->ref();
}

void
memory_base::
do_set_debug_write_callback(memory_debug_callback_base* cb)
{
  assert(mWriteCallback);
  assert(cb);
  mDebugWriteCallback->unref();
  mDebugWriteCallback = cb;
  mDebugWriteCallback->ref();
}

void
memory_base::
do_set_read_restriction(memory_access_restriction_base* r)
{
  assert(mReadRestriction);
  assert(r);
  if (!mReadRestriction->has_default_restriction()) {
    read_restriction_removed();
  }
  mReadRestriction->unref();
  mReadRestriction = r;
  if (!mReadRestriction->has_default_restriction()) {
    read_restriction_added();
  }
  mReadRestriction->ref();
}

void
memory_base::
do_set_write_restriction(memory_access_restriction_base* r)
{
  assert(mWriteRestriction);
  assert(r);
  if (!mWriteRestriction->has_default_restriction()) {
    write_restriction_removed();
  }
  mWriteRestriction->unref();
  mWriteRestriction = r;
  if (!mWriteRestriction->has_default_restriction()) {
    write_restriction_added();
  }
  mWriteRestriction->ref();
}

void
memory_base::
set_callback(memory_callback_base* cb)
{
  assert(cb);
  scoped_reference<memory_callback_base> scopedRef(cb);
  if (has_default_write_behavior() || has_default_read_behavior()) {
    invalidate_dmi();
  }
  do_set_read_callback(cb);
  do_set_write_callback(cb);
  mCallbackObservers.notify_callback_changed(ACCESS_TYPE_ACCESS, ACCESS_MODE_REGULAR);
}

void
memory_base::
set_read_callback(memory_callback_base* cb)
{
  assert(cb);
  scoped_reference<memory_callback_base> scopedRef(cb);
  if (has_default_read_behavior()) {
    invalidate_dmi();
  }
  do_set_read_callback(cb);
  mCallbackObservers.notify_callback_changed(ACCESS_TYPE_READ, ACCESS_MODE_REGULAR);
}

void
memory_base::
set_write_callback(memory_callback_base* cb)
{
  assert(cb);
  scoped_reference<memory_callback_base> scopedRef(cb);
  if (has_default_write_behavior()) {
    invalidate_dmi();
  }
  do_set_write_callback(cb);
  mCallbackObservers.notify_callback_changed(ACCESS_TYPE_WRITE, ACCESS_MODE_REGULAR);
}

void
memory_base::
remove_callback()
{
  if (!has_default_write_behavior() || !has_default_read_behavior()) {
    scoped_reference<memory_callback_base> scopedRef(mDefaultCallback);
    do_set_read_callback(mDefaultCallback);
    do_set_write_callback(mDefaultCallback);
    invalidate_dmi();
    mCallbackObservers.notify_callback_changed(ACCESS_TYPE_ACCESS, ACCESS_MODE_REGULAR);
  }
}

void
memory_base::
remove_read_callback()
{
  if (!has_default_read_behavior()) {
    scoped_reference<memory_callback_base> scopedRef(mDefaultCallback);
    do_set_read_callback(mDefaultCallback);
    invalidate_dmi();
    mCallbackObservers.notify_callback_changed(ACCESS_TYPE_READ, ACCESS_MODE_REGULAR);
  }
}

void
memory_base::
remove_write_callback()
{
  if (!has_default_write_behavior()) {
    scoped_reference<memory_callback_base> scopedRef(mDefaultCallback);
    do_set_write_callback(mDefaultCallback);
    invalidate_dmi();
    mCallbackObservers.notify_callback_changed(ACCESS_TYPE_WRITE, ACCESS_MODE_REGULAR);
  }
}

memory_callback_base*
memory_base::
get_read_callback() const
{
  return mReadCallback;
}

memory_callback_base*
memory_base::
get_write_callback() const
{
  return mWriteCallback;
}

memory_access_restriction_base* 
memory_base::
get_read_restriction() const
{
  return mReadRestriction;
}

memory_access_restriction_base* 
memory_base::
get_write_restriction() const
{
  return mWriteRestriction;
}
  
void
memory_base::
set_restriction(memory_access_restriction_base* r)
{
  assert(r);
  scoped_reference<memory_access_restriction_base> scopedRef(r);
  if (has_default_write_restriction() || has_default_read_restriction()) {
    invalidate_dmi();
  }
  do_set_read_restriction(r);
  do_set_write_restriction(r);
}


void 
memory_base::
set_read_restriction(memory_access_restriction_base* r)
{
  assert(mReadRestriction);
  assert(r);
  scoped_reference<memory_access_restriction_base> scopedRef(r);
  if (has_default_read_restriction()) {
    invalidate_dmi();
  }
  do_set_read_restriction(r);
}

void 
memory_base::
set_write_restriction(memory_access_restriction_base* r)
{
  assert(mWriteRestriction);
  assert(r);
  scoped_reference<memory_access_restriction_base> scopedRef(r);
  if (has_default_write_restriction()) {
    invalidate_dmi();
  }
  do_set_write_restriction(r);
}

void
memory_base::
remove_read_restriction()
{
  if (!has_default_read_restriction()) {
    scoped_reference<memory_access_restriction_base> scopedRef(mDefaultRestriction);
    do_set_read_restriction(mDefaultRestriction);
    invalidate_dmi();
  }
}

void
memory_base::
remove_write_restriction()
{
  if (!has_default_write_restriction()) {
    scoped_reference<memory_access_restriction_base> scopedRef(mDefaultRestriction);
    do_set_write_restriction(mDefaultRestriction);
    invalidate_dmi();
  }
}


void
memory_base::
set_debug_callback(memory_debug_callback_base* cb)
{
  assert(cb);
  scoped_reference<memory_debug_callback_base> scopedRef(cb);
  if (has_default_debug_write_behavior() || has_default_debug_read_behavior()) {
    invalidate_dmi();
  }
  do_set_debug_read_callback(cb);
  do_set_debug_write_callback(cb);
  mCallbackObservers.notify_callback_changed(ACCESS_TYPE_ACCESS, ACCESS_MODE_DEBUG);
}

void
memory_base::
set_debug_read_callback(memory_debug_callback_base* cb)
{
  assert(cb);
  scoped_reference<memory_debug_callback_base> scopedRef(cb);
  if (has_default_debug_read_behavior()) {
    invalidate_dmi();
  }
  do_set_debug_read_callback(cb);
  mCallbackObservers.notify_callback_changed(ACCESS_TYPE_READ, ACCESS_MODE_DEBUG);
}

void
memory_base::
set_debug_write_callback(memory_debug_callback_base* cb)
{
  assert(cb);
  scoped_reference<memory_debug_callback_base> scopedRef(cb);
  if (has_default_debug_write_behavior()) {
    invalidate_dmi();
  }
  do_set_debug_write_callback(cb);
  mCallbackObservers.notify_callback_changed(ACCESS_TYPE_WRITE, ACCESS_MODE_DEBUG);
}

void
memory_base::
remove_debug_callback()
{
  if (!has_default_debug_write_behavior() || !has_default_debug_read_behavior()) {
    scoped_reference<memory_debug_callback_base> scopedRef(mDefaultDebugCallback);
    do_set_debug_read_callback(mDefaultDebugCallback);
    do_set_debug_write_callback(mDefaultDebugCallback);
    invalidate_dmi();
    mCallbackObservers.notify_callback_changed(ACCESS_TYPE_ACCESS, ACCESS_MODE_DEBUG);
  }
}

void
memory_base::
remove_debug_read_callback()
{
  if (!has_default_debug_read_behavior()) {
    scoped_reference<memory_debug_callback_base> scopedRef(mDefaultDebugCallback);
    do_set_debug_read_callback(mDefaultDebugCallback);
    invalidate_dmi();
    mCallbackObservers.notify_callback_changed(ACCESS_TYPE_READ, ACCESS_MODE_DEBUG);
  }
}

void
memory_base::
remove_debug_write_callback()
{
  if (!has_default_debug_write_behavior()) {
    scoped_reference<memory_debug_callback_base> scopedRef(mDefaultDebugCallback);
    do_set_debug_write_callback(mDefaultDebugCallback);
    invalidate_dmi();
    mCallbackObservers.notify_callback_changed(ACCESS_TYPE_WRITE, ACCESS_MODE_DEBUG);
  }
}

memory_debug_callback_base*
memory_base::
get_debug_read_callback() const
{
  return mDebugReadCallback;
}

memory_debug_callback_base*
memory_base::
get_debug_write_callback() const
{
  return mDebugWriteCallback;
}

unsigned int
memory_base::
collect_bitfields(std::vector<bitfield_base*>&) const
{
  return 0;
}

bool
memory_base::
content_is_observable() const
{
  return true;
}

void
memory_base::
enable_dmi()
{
  if (!is_dmi_enabled()) {
    invalidate_dmi();
    mDmiEnabled = true;
  }
}

void
memory_base::
disable_dmi()
{
  if (is_dmi_enabled()) {
    invalidate_dmi();
    mDmiEnabled = false;
  }
}

bool
memory_base::
is_dmi_enabled() const
{
  return mDmiEnabled;
}

bool
memory_base::
set_symbolic_name(const std::vector<unsigned char>& value,
                  const std::string& name)
{
  return mSymbolicNames.set_symbolic_name(value, name);
}

std::string
memory_base::
get_symbolic_name(const std::vector<unsigned char>& value) const {
  return mSymbolicNames.get_symbolic_name(value);
}

unsigned int
memory_base::
collect_symbolic_names(SymbolicNames& symbolicNames) const
{
  return mSymbolicNames.collect_symbolic_names(symbolicNames);
}

void 
memory_base::
filter(tlm::tlm_generic_payload& trans) const
{
  if (are_bytes_enabled(trans, get_size()*get_width())) {
    trans_address_helper helper(trans, index_to_address(get_offset()-(get_parent() ? get_parent()->get_offset() : 0), get_toplevel()->get_width()));
    memory_access_restriction_base* restriction = trans.is_read() ? 
      get_read_restriction() : get_write_restriction();
    if (!restriction->has_default_restriction()) {
      if (restriction->restrict(trans)) {
	trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
	return;
      }
    }
  
    const unsigned long long address = trans.get_address();
    const unsigned long long lowIndex = address_to_index(address, get_width())+get_offset();
    const unsigned long long highIndex = address_to_index
      (address + std::min(trans.get_data_length(), trans.get_streaming_width())-1,
       get_width())+get_offset();
    for (AliasesConstIterator i = begin_alias(); i != end_alias(); ++i) {
      if (lowIndex*get_width() >= (*i)->get_offset()*get_width() && 
	  highIndex*get_width() < ((*i)->get_offset()*get_width() +
				   (*i)->get_size()*(*i)->get_width())) {
	(*i)->filter(trans);
      }
    }
  
    if (trans.get_response_status() != tlm::TLM_GENERIC_ERROR_RESPONSE) {
      post_filter(trans);
    }
  }
  if (trans.get_response_status() != tlm::TLM_GENERIC_ERROR_RESPONSE) {
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
  }
}

void 
memory_base::
post_filter(tlm::tlm_generic_payload& /*trans*/) const
{
}


}
