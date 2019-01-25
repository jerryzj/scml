/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/reg.h>

#include <scml2/memory_default_callback.h>
#include <scml2/memory_default_debug_callback.h>
#include <scml2/memory_bitfields_read_callback.h>
#include <scml2/memory_bitfields_write_callback.h>
#include <scml2/memory_bitfields_debug_read_callback.h>
#include <scml2/memory_bitfields_debug_write_callback.h>
#include <scml2/memory_default_access_restriction.h>

#include <scml2/bitfield.h>
#include <scml2/memory_alias.h>
#include <scml2/memory.h>

#include <scml2_logging/stream.h>

#include "tagged_message.h"

namespace scml2 {

template <typename DT>
reg<DT>::
reg(const std::string& name,
    toplevel_memory_base& parent,
    unsigned long long offset) :
  memory_base(name, &parent, offset, 1, sizeOf<DT>()),
  mToplevel(parent),
  mFastAccess(),
  mHasUserReadCallback(false),
  mHasUserWriteCallback(false),
  mHasUserDebugReadCallback(false),
  mHasUserDebugWriteCallback(false),
  mHasDefaultBitfieldReadBehavior(true),
  mHasDefaultBitfieldWriteBehavior(true),
  mHasDefaultBitfieldDebugReadBehavior(true),
  mHasDefaultBitfieldDebugWriteBehavior(true),
  mHasBitfieldPostWriteBehavior(false)
{
  setup();
}

template <typename DT>
reg<DT>::
reg(const std::string& name,
    memory_alias_base& parent,
    unsigned long long offset) :
  memory_base(name, &parent, parent.get_offset() + offset, 1, sizeOf<DT>()),
  mToplevel(parent.mToplevel),
  mFastAccess(),
  mHasUserReadCallback(false),
  mHasUserWriteCallback(false),
  mHasUserDebugReadCallback(false),
  mHasUserDebugWriteCallback(false),
  mHasDefaultBitfieldReadBehavior(true),
  mHasDefaultBitfieldWriteBehavior(true),
  mHasDefaultBitfieldDebugReadBehavior(true),
  mHasDefaultBitfieldDebugWriteBehavior(true),
  mHasBitfieldPostWriteBehavior(false)
{
  setup();
}

template <typename DT>
reg<DT>::
~reg()
{
}

template <typename DT>
void
reg<DT>::
initialize(const DT& value)
{
  put_debug(value);
}

template <typename DT>
bool
reg<DT>::
set_symbolic_name(const DT& value, const std::string& name)
{
  std::vector<unsigned char> tmp(sizeOf<DT>());
  to_char_array(value, &tmp[0], sizeOf<DT>(), 0);
  return set_symbolic_name(tmp, name);
}

template <typename DT>
std::string
reg<DT>::
get_symbolic_name(const DT& value)
{
  std::vector<unsigned char> tmp(sizeOf<DT>());
  to_char_array(value, &tmp[0], sizeOf<DT>(), 0);
  return get_symbolic_name(tmp);
}

template <typename DT>
void
reg<DT>::
setup()
{
  init_streams(mToplevel.get_streams());

  init_default_callback(new memory_default_callback<DT>(mFastAccess, mToplevel));
  init_default_debug_callback(new memory_default_debug_callback<DT>(mFastAccess));
  init_default_restriction(new memory_default_access_restriction<DT>());

  update_fast_access();
}

template <typename DT>
void
reg<DT>::
invalidate_dmi()
{
  mToplevel.invalidate_dmi();
}

template <typename DT>
void
reg<DT>::
update_fast_access()
{
  mFastAccess.update(mToplevel.template create_slow_access<DT>(get_offset()));
}

template <typename DT>
void
reg<DT>::
set_callback(memory_callback_base* cb)
{
  memory_base::set_callback(cb);
  mHasUserReadCallback = true;
  mHasUserWriteCallback = true;
}

template <typename DT>
void
reg<DT>::
set_read_callback(memory_callback_base* cb)
{
  memory_base::set_read_callback(cb);
  mHasUserReadCallback = true;
}

template <typename DT>
void
reg<DT>::
set_write_callback(memory_callback_base* cb)
{
  memory_base::set_write_callback(cb);
  mHasUserWriteCallback = true;
}

template <typename DT>
void
reg<DT>::
remove_callback()
{
  if (mHasDefaultBitfieldReadBehavior && mHasDefaultBitfieldWriteBehavior) {
    memory_base::remove_callback();

  } else if (mHasDefaultBitfieldReadBehavior) {
    memory_base::set_write_callback(new memory_bitfields_write_callback<DT>(*this));
    memory_base::remove_read_callback();

  } else if (mHasDefaultBitfieldWriteBehavior) {
    memory_base::set_read_callback(new memory_bitfields_read_callback<DT>(*this));
    memory_base::remove_write_callback();

  } else {
    memory_base::set_read_callback(new memory_bitfields_read_callback<DT>(*this));
    memory_base::set_write_callback(new memory_bitfields_write_callback<DT>(*this));
  }
  mHasUserReadCallback = false;
  mHasUserWriteCallback = false;
}

template <typename DT>
void
reg<DT>::
remove_read_callback()
{
  if (mHasDefaultBitfieldReadBehavior) {
    memory_base::remove_read_callback();

  } else {
    memory_base::set_read_callback(new memory_bitfields_read_callback<DT>(*this));
  }
  mHasUserReadCallback = false;
}

template <typename DT>
void
reg<DT>::
remove_write_callback()
{
  if (mHasDefaultBitfieldWriteBehavior) {
    memory_base::remove_write_callback();

  } else {
    memory_base::set_write_callback(new memory_bitfields_write_callback<DT>(*this));
  }
  mHasUserWriteCallback = false;
}

template <typename DT>
void
reg<DT>::
set_debug_callback(memory_debug_callback_base* cb)
{
  memory_base::set_debug_callback(cb);
  mHasUserDebugReadCallback = true;
  mHasUserDebugWriteCallback = true;
}

template <typename DT>
void
reg<DT>::
set_debug_read_callback(memory_debug_callback_base* cb)
{
  memory_base::set_debug_read_callback(cb);
  mHasUserDebugReadCallback = true;
}

template <typename DT>
void
reg<DT>::
set_debug_write_callback(memory_debug_callback_base* cb)
{
  memory_base::set_debug_write_callback(cb);
  mHasUserDebugWriteCallback = true;
}

template <typename DT>
void
reg<DT>::
remove_debug_callback()
{
  if (mHasDefaultBitfieldDebugReadBehavior && mHasDefaultBitfieldDebugWriteBehavior) {
    memory_base::remove_debug_callback();

  } else if (mHasDefaultBitfieldDebugReadBehavior) {
    memory_base::set_debug_write_callback(new memory_bitfields_debug_write_callback<DT>(*this));
    memory_base::remove_debug_read_callback();

  } else if (mHasDefaultBitfieldDebugWriteBehavior) {
    memory_base::set_debug_read_callback(new memory_bitfields_debug_read_callback<DT>(*this));
    memory_base::remove_debug_write_callback();

  } else {
    memory_base::set_debug_read_callback(new memory_bitfields_debug_read_callback<DT>(*this));
    memory_base::set_debug_write_callback(new memory_bitfields_debug_write_callback<DT>(*this));
  }
  mHasUserDebugReadCallback = false;
  mHasUserDebugWriteCallback = false;
}

template <typename DT>
void
reg<DT>::
remove_debug_read_callback()
{
  if (mHasDefaultBitfieldDebugReadBehavior) {
    memory_base::remove_debug_read_callback();

  } else {
    memory_base::set_debug_read_callback(new memory_bitfields_debug_read_callback<DT>(*this));
  }
  mHasUserDebugReadCallback = false;
}

template <typename DT>
void
reg<DT>::
remove_debug_write_callback()
{
  if (mHasDefaultBitfieldDebugWriteBehavior) {
    memory_base::remove_debug_write_callback();

  } else {
    memory_base::set_debug_write_callback(new memory_bitfields_debug_write_callback<DT>(*this));
  }
  mHasUserDebugWriteCallback = false;
}

template <typename DT>
void
reg<DT>::
register_bitfield(bitfield<DT>& bitfield)
{
  if ((bitfield.get_offset() + bitfield.get_size()) > (sizeOf<DT>() * 8)) {
    SCML2_LOG_ERROR(this, get_streams().error(), CONFIGURATION_ERROR)
      << "The bitfield '" << bitfield.get_name()
      << "' addresses bits outside the range of the parent reg '"
      << get_name() << "'"
      << std::endl;
  }

  mBitfieldContainer.register_bitfield(bitfield);
}

template <typename DT>
inline
void
reg<DT>::
bitfield_read_callback_registered()
{
  if (has_default_read_behavior()) {
    memory_base::set_read_callback(new memory_bitfields_read_callback<DT>(*this));
  }
  mHasDefaultBitfieldReadBehavior = false;
}

template <typename DT>
inline
void
reg<DT>::
bitfield_write_callback_registered()
{
  if (has_default_write_behavior()) {
    memory_base::set_write_callback(new memory_bitfields_write_callback<DT>(*this));
  }
  mHasDefaultBitfieldWriteBehavior = false;
}

template <typename DT>
inline
void
reg<DT>::
bitfield_post_write_callback_registered()
{
  if (has_default_write_behavior()) {
    memory_base::set_write_callback(new memory_bitfields_write_callback<DT>(*this));
  }
  mHasBitfieldPostWriteBehavior = true;
}

template <typename DT>
inline
void
reg<DT>::
bitfield_read_callback_unregistered()
{
  if (mBitfieldContainer.has_default_read_behavior()) {
    if (!mHasUserReadCallback) {
      memory_base::remove_read_callback();
    }
    mHasDefaultBitfieldReadBehavior = true;
  }
}

template <typename DT>
inline
void
reg<DT>::
bitfield_write_callback_unregistered()
{
  if (mBitfieldContainer.has_default_write_behavior()) {
    if (!mHasUserWriteCallback) {
      memory_base::remove_write_callback();
    }
    mHasDefaultBitfieldWriteBehavior = true;
  }
}

template <typename DT>
inline
void
reg<DT>::
bitfield_post_write_callback_unregistered()
{
  /* koen
  if (mBitfieldContainer.has_default_write_behavior()) {
    if (!mHasUserWriteCallback) {
      memory_base::remove_write_callback();
    }
    mHasDefaultBitfieldWriteBehavior = true;
  }
  */
}

template <typename DT>
inline
void
reg<DT>::
bitfield_debug_read_callback_registered()
{
  if (has_default_debug_read_behavior()) {
    memory_base::set_debug_read_callback(new memory_bitfields_debug_read_callback<DT>(*this));
  }
  mHasDefaultBitfieldDebugReadBehavior = false;
}

template <typename DT>
inline
void
reg<DT>::
bitfield_debug_write_callback_registered()
{
  if (has_default_debug_write_behavior()) {
    memory_base::set_debug_write_callback(new memory_bitfields_debug_write_callback<DT>(*this));
  }
  mHasDefaultBitfieldDebugWriteBehavior = false;
}

template <typename DT>
inline
void
reg<DT>::
bitfield_debug_read_callback_unregistered()
{
  if (mBitfieldContainer.has_default_debug_read_behavior()) {
    if (!mHasUserDebugReadCallback) {
      memory_base::remove_debug_read_callback();
    }
    mHasDefaultBitfieldDebugReadBehavior = true;
  }
}

template <typename DT>
inline
void
reg<DT>::
bitfield_debug_write_callback_unregistered()
{
  if (mBitfieldContainer.has_default_debug_write_behavior()) {
    if (!mHasUserDebugWriteCallback) {
      memory_base::remove_debug_write_callback();
    }
    mHasDefaultBitfieldDebugWriteBehavior = true;
  }
}

template <typename DT>
unsigned int
reg<DT>::
read(unsigned long long address,
     unsigned char* data,
     unsigned int size)
{
  return mToplevel.read(address + mToplevel.get_width()*get_offset(),
                        data,
                        size);
}

template <typename DT>
unsigned int
reg<DT>::
write(unsigned long long address,
      const unsigned char* data,
      unsigned int size)
{
  return mToplevel.write(address + mToplevel.get_width()*get_offset(),
                         data,
                         size);
}

template <typename DT>
bool
reg<DT>::
register_content_observer(memory_content_observer* o)
{
  return mToplevel.register_content_observer(o, get_offset());
}

template <typename DT>
bool
reg<DT>::
unregister_all_content_observers()
{
  return mToplevel.unregister_all_content_observers();
}

template <typename DT>
bool
reg<DT>::
unregister_content_observer(memory_content_observer* o)
{
  return mToplevel.unregister_content_observer(o);
}


template <typename DT>
unsigned int
reg<DT>::
collect_bitfields(std::vector<bitfield_base*>& b) const
{
  return mBitfieldContainer.collect_bitfields(b);
}

template <typename DT>
void
reg<DT>::
post_filter(tlm::tlm_generic_payload& trans) const
{
  assert(trans.get_response_status() != tlm::TLM_GENERIC_ERROR_RESPONSE);
  assert(trans.get_address() == 0);
  assert(trans.get_byte_enable_ptr() != 0);
  assert(trans.get_byte_enable_length() >= sizeOf<DT>());
  if (mBitfieldContainer.filter(trans.is_read(), *reinterpret_cast<DT*>(trans.get_data_ptr()), *reinterpret_cast<DT*>(trans.get_byte_enable_ptr()))) {
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
  }
}

template <typename DT>
const char*
reg<DT>::
kind() const
{
  return "scml2::reg";
}
#ifdef _WIN32
#define SCML2_INSTANTIATE_TEMPLATE(type) template class SNPS_VP_API reg<type >;
#else
#define SCML2_INSTANTIATE_TEMPLATE(type) template class reg<type >;
#endif  
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}
