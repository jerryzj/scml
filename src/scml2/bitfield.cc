/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/bitfield.h>

#include <scml2/bitfield_default_read_callback.h>
#include <scml2/bitfield_default_write_callback.h>
#include <scml2/bitfield_default_debug_read_callback.h>
#include <scml2/bitfield_default_debug_write_callback.h>
#include <scml2/bitfield_default_post_write_callback.h>
#include <scml2/bitfield_read_callback_base.h>
#include <scml2/bitfield_write_callback_base.h>
#include <scml2/bitfield_debug_write_callback_base.h>
#include <scml2/bitfield_debug_read_callback_base.h>
#include <scml2/bitfield_enum_post_write_decoder.h>
#include <scml2/bitfield_default_restriction.h>
#include <scml2/reg.h>

#include <cassert>

namespace scml2 {

template <typename DT>
bitfield<DT>::
bitfield(const std::string& name,
         reg<DT>& r,
         unsigned int offset,
         unsigned int size) :
  bitfield_base(name, offset, size, r.get_streams()),
  mRegister(r),
  mParentBitfield(0)
{
  setup();
  mRegister.register_bitfield(*this);
}

template <typename DT>
restriction_owner* 
bitfield<DT>::
get_restriction_parent() const
{
  if (mParentBitfield != 0) {
    return mParentBitfield;
  } else {
    return &mRegister;
  }
}

template <typename DT>
const reg<DT>& 
bitfield<DT>::get_register() const
{
  return mRegister;
}

template <typename DT>
reg<DT>& 
bitfield<DT>::get_register()
{
  return mRegister;
}

template <typename DT>
void 
bitfield<DT>::store_old_value() {
  std::cerr << "illegal use of undocumented API bitfield<DT>::store_old_value()" << std::endl;
  abort();
}

template <typename DT>
DT 
bitfield<DT>::get_old_value() const{
  std::cerr << "illegal use of undocumented API bitfield<DT>::get_old_value()" << std::endl;
  abort();
  return (DT)0;
}

template <typename DT>
bitfield<DT>::
bitfield(const std::string& name,
         bitfield<DT>& b,
         unsigned int offset,
         unsigned int size) :
  bitfield_base(name, b.get_offset() + offset, size, b.get_streams()),
  mRegister(b.mRegister),
  mParentBitfield(&b)
{
  setup();
  mParentBitfield->register_bitfield(*this);
}


template <typename DT>
bitfield<DT>::
~bitfield()
{
  mDefaultReadCallback->unref();
  mDefaultWriteCallback->unref();
  mDefaultDebugReadCallback->unref();
  mDefaultDebugWriteCallback->unref();
  mDefaultPostWriteCallback->unref();
  mReadCallback->unref();
  mWriteCallback->unref();
  mDebugReadCallback->unref();
  mDebugWriteCallback->unref();
  mPostWriteCallback->unref();
}

template <typename DT>
void
bitfield<DT>::
setup()
{
  mDefaultReadCallback = new bitfield_default_read_callback<DT>(*this);
  mDefaultReadCallback->ref();
  mReadCallback = mDefaultReadCallback;
  mReadCallback->ref();

  mDefaultWriteCallback = new bitfield_default_write_callback<DT>(*this);
  mDefaultWriteCallback->ref();
  mWriteCallback = mDefaultWriteCallback;
  mWriteCallback->ref();

  mDefaultDebugReadCallback = new bitfield_default_debug_read_callback<DT>(*this);
  mDefaultDebugReadCallback->ref();
  mDebugReadCallback = mDefaultDebugReadCallback;
  mDebugReadCallback->ref();

  mDefaultDebugWriteCallback = new bitfield_default_debug_write_callback<DT>(*this);
  mDefaultDebugWriteCallback->ref();
  mDebugWriteCallback = mDefaultDebugWriteCallback;
  mDebugWriteCallback->ref();
  
  mDefaultPostWriteCallback = new bitfield_default_post_write_callback<DT>(*this);
  mDefaultPostWriteCallback->ref();
  mPostWriteCallback = mDefaultPostWriteCallback;
  mPostWriteCallback->ref();

  mDefaultRestriction = new bitfield_default_restriction<DT>(*this);
  mDefaultRestriction->ref();
  mReadRestriction = mDefaultRestriction;
  mReadRestriction->ref();
  mWriteRestriction = mDefaultRestriction;
  mWriteRestriction->ref();
  
}

template <typename DT>
void
bitfield<DT>::
register_bitfield(bitfield<DT>& bitfield)
{
  if ((bitfield.get_offset() + bitfield.get_size()) > (get_offset() + get_size())) {
    SCML2_LOG_ERROR(this, get_streams().error(), CONFIGURATION_ERROR)
      << "The bitfield '" << bitfield.get_name()
      << "' addresses bits outside the range of the parent bitfield '"
      << get_name() << "'"
      << std::endl;
  }

  mBitfieldContainer.register_bitfield(bitfield);
}

template <typename DT>
bool
bitfield<DT>::
set_symbolic_name(const DT& value, const std::string& name)
{
  std::vector<unsigned char> tmp(sizeOf<DT>());
  to_char_array(value, &tmp[0], sizeOf<DT>(), 0);
  return set_symbolic_name(tmp, name);
}

template <typename DT>
std::string
bitfield<DT>::
get_symbolic_name(const DT& value)
{
  std::vector<unsigned char> tmp(sizeOf<DT>());
  to_char_array(value, &tmp[0], sizeOf<DT>(), 0);
  return get_symbolic_name(tmp);
}

template <typename DT>
void
bitfield<DT>::
do_set_read_callback(bitfield_read_callback_base<DT>* cb)
{
  assert(mReadCallback);
  assert(cb);
  mReadCallback->unref();
  mReadCallback = cb;
  mReadCallback->ref();
}

template <typename DT>
void
bitfield<DT>::
do_set_write_callback(bitfield_write_callback_base<DT>* cb)
{
  assert(mWriteCallback);
  assert(cb);
  mWriteCallback->unref();
  mWriteCallback = cb;
  mWriteCallback->ref();
}

template <typename DT>
void
bitfield<DT>::
do_set_read_restriction(bitfield_access_restriction_base<DT>* r)
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

template <typename DT>
void
bitfield<DT>::
do_set_write_restriction(bitfield_access_restriction_base<DT>* r)
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

template <typename DT>
void
bitfield<DT>::
do_set_post_write_callback(bitfield_post_write_callback_base<DT>* cb)
{
  assert(mPostWriteCallback);
  assert(cb);
  mPostWriteCallback->unref();
  mPostWriteCallback = cb;
  mPostWriteCallback->ref();
}

template <typename DT>
void
bitfield<DT>::
do_set_post_write_callback(DT enumval, bitfield_post_write_callback_base<DT>* cb){
  //check if we the current callback is an enum decoder callback. 
  assert(mPostWriteCallback);
  assert(cb);
  if (! dynamic_cast< bitfield_enum_post_write_decoder<DT> * > (mPostWriteCallback)) {
    //we don't have a decoder yet. instantiate one.
    mPostWriteCallback->unref();
    mPostWriteCallback = new bitfield_enum_post_write_decoder<DT>("");
    mPostWriteCallback->ref();
  } 
  bitfield_enum_post_write_decoder<DT> * decoder =  dynamic_cast< bitfield_enum_post_write_decoder<DT> *> (mPostWriteCallback);
  if (decoder) {
    cb->ref();
    decoder->register_decode_cb(enumval, cb);
  } else {
    //this is utterly inexplicable.
    assert(0);
  }
}

template <typename DT>
void
bitfield<DT>::
do_set_default_enum_write_callback(bitfield_post_write_callback_base<DT>* cb){
  //check if we the current callback is an enum decoder callback. 
  assert(mPostWriteCallback);
  assert(cb);
  if (! dynamic_cast< bitfield_enum_post_write_decoder<DT> * > (mPostWriteCallback)) {
    //we don't have a decoder yet. instantiate one.
    mPostWriteCallback->unref();
    mPostWriteCallback = new bitfield_enum_post_write_decoder<DT>("");
    mPostWriteCallback->ref();
  } else {
  }
  bitfield_enum_post_write_decoder<DT> * decoder =  dynamic_cast< bitfield_enum_post_write_decoder<DT> *> (mPostWriteCallback);
  if (decoder) {
    cb->ref();
    decoder->register_default_cb(cb);
  } else {
    //this is utterly inexplicable.
    assert(0);
  }
}


template <typename DT>
void
bitfield<DT>::
do_set_debug_read_callback(bitfield_debug_read_callback_base<DT>* cb)
{
  assert(mDebugReadCallback);
  assert(cb);
  mDebugReadCallback->unref();
  mDebugReadCallback = cb;
  mDebugReadCallback->ref();
}

template <typename DT>
void
bitfield<DT>::
do_set_debug_write_callback(bitfield_debug_write_callback_base<DT>* cb)
{
  assert(mDebugWriteCallback);
  assert(cb);
  mDebugWriteCallback->unref();
  mDebugWriteCallback = cb;
  mDebugWriteCallback->ref();
}

template <typename DT>
void
bitfield<DT>::
set_read_callback(bitfield_read_callback_base<DT>* cb)
{
  assert(cb);
  scoped_reference<bitfield_read_callback_base<DT> > scopedRef(cb);
  if (has_default_read_behavior()) {
    mRegister.bitfield_read_callback_registered();
  }
  do_set_read_callback(cb);
  mCallbackObservers.notify_callback_changed(ACCESS_TYPE_READ, ACCESS_MODE_REGULAR);
}

template <typename DT>
void
bitfield<DT>::
set_write_callback(bitfield_write_callback_base<DT>* cb)
{
  assert(cb);
  scoped_reference<bitfield_write_callback_base<DT> > scopedRef(cb);
  if (has_default_write_behavior()) {
    mRegister.bitfield_write_callback_registered();
  }
  do_set_write_callback(cb);
  mCallbackObservers.notify_callback_changed(ACCESS_TYPE_WRITE, ACCESS_MODE_REGULAR);
}

template <typename DT>
void
bitfield<DT>::
set_restriction(bitfield_access_restriction_base<DT>* r)
{
  assert(r);
  scoped_reference<bitfield_access_restriction_base<DT> > scopedRef(r);
  if (has_default_read_restriction() || has_default_write_restriction()) {
    get_register().invalidate_dmi();
  }
  do_set_read_restriction(r);
  do_set_write_restriction(r);
}

template <typename DT>
void
bitfield<DT>::
set_read_restriction(bitfield_access_restriction_base<DT>* r)
{
  assert(r);
  scoped_reference<bitfield_access_restriction_base<DT> > scopedRef(r);
  if (has_default_read_restriction()) {
    get_register().invalidate_dmi();
  }
  do_set_read_restriction(r);
}

template <typename DT>
void
bitfield<DT>::
set_write_restriction(bitfield_access_restriction_base<DT>* r)
{
  assert(r);
  scoped_reference<bitfield_access_restriction_base<DT> > scopedRef(r);
  if (has_default_write_restriction()) {
    get_register().invalidate_dmi();
  }
  do_set_write_restriction(r);
}

template <typename DT>
void
bitfield<DT>::
set_post_write_callback(bitfield_post_write_callback_base<DT>* cb)
{
  assert(cb);
  scoped_reference<bitfield_post_write_callback_base<DT> > scopedRef(cb);
  
  mRegister.bitfield_post_write_callback_registered();

  do_set_post_write_callback(cb);
  
  mCallbackObservers.notify_callback_changed(ACCESS_TYPE_WRITE, ACCESS_MODE_REGULAR);
}

template <typename DT>
void
bitfield<DT>::
set_post_write_callback(DT enum_val, bitfield_post_write_callback_base<DT>* cb){
  assert(cb);
  scoped_reference<bitfield_post_write_callback_base<DT> > scopedRef(cb);
  mRegister.bitfield_post_write_callback_registered();

  do_set_post_write_callback(enum_val, cb);
  
  mCallbackObservers.notify_callback_changed(ACCESS_TYPE_WRITE, ACCESS_MODE_REGULAR);
}

template <typename DT>
void
bitfield<DT>::
set_default_enum_write_callback(bitfield_post_write_callback_base<DT>* cb){
  assert(cb);
  scoped_reference<bitfield_post_write_callback_base<DT> > scopedRef(cb);
  mRegister.bitfield_post_write_callback_registered();

  do_set_default_enum_write_callback(cb);
  
  mCallbackObservers.notify_callback_changed(ACCESS_TYPE_WRITE, ACCESS_MODE_REGULAR);

}

template <typename DT>
void
bitfield<DT>::
remove_callback()
{
  remove_read_callback();
  remove_write_callback();
}

template <typename DT>
void
bitfield<DT>::
remove_read_callback()
{
  if (!has_default_read_behavior()) {
    scoped_reference<bitfield_read_callback_base<DT> > scopedRef(mDefaultReadCallback);
    do_set_read_callback(mDefaultReadCallback);
    mRegister.bitfield_read_callback_unregistered();
    mCallbackObservers.notify_callback_changed(ACCESS_TYPE_READ, ACCESS_MODE_REGULAR);
  }
}

template <typename DT>
void
bitfield<DT>::
remove_write_callback()
{
  if (!has_default_write_behavior()) {
    scoped_reference<bitfield_write_callback_base<DT> > scopedRef(mDefaultWriteCallback);
    do_set_write_callback(mDefaultWriteCallback);
    mRegister.bitfield_write_callback_unregistered();
    mCallbackObservers.notify_callback_changed(ACCESS_TYPE_WRITE, ACCESS_MODE_REGULAR);
  }
}

template <typename DT>
void
bitfield<DT>::
remove_restriction()
{
  remove_read_restriction();
  remove_write_restriction();
}

template <typename DT>
void
bitfield<DT>::
remove_read_restriction()
{
  if (!has_default_read_restriction()) {
    scoped_reference<bitfield_access_restriction_base<DT> > scopedRef(mDefaultRestriction);
    do_set_read_restriction(mDefaultRestriction);
    get_register().invalidate_dmi();
  }
}

template <typename DT>
void
bitfield<DT>::
remove_write_restriction()
{
  if (!has_default_write_restriction()) {
    scoped_reference<bitfield_access_restriction_base<DT> > scopedRef(mDefaultRestriction);
    do_set_write_restriction(mDefaultRestriction);
    get_register().invalidate_dmi();
  }
}

template <typename DT>
bitfield_read_callback_base<DT>*
bitfield<DT>::
get_read_callback() const
{
  return mReadCallback;
}

template <typename DT>
bitfield_write_callback_base<DT>*
bitfield<DT>::
get_write_callback() const
{
  return mWriteCallback;
}

template <typename DT>
bitfield_access_restriction_base<DT>*
bitfield<DT>::
get_read_restriction() const
{
  return mReadRestriction;
}

template <typename DT>
bitfield_access_restriction_base<DT>*
bitfield<DT>::
get_write_restriction() const
{
  return mWriteRestriction;
}

template <typename DT>
void
bitfield<DT>::
set_debug_read_callback(bitfield_debug_read_callback_base<DT>* cb)
{
  assert(cb);
  scoped_reference<bitfield_debug_read_callback_base<DT> > scopedRef(cb);
  if (has_default_debug_read_behavior()) {
    mRegister.bitfield_debug_read_callback_registered();
  }
  do_set_debug_read_callback(cb);
  mCallbackObservers.notify_callback_changed(ACCESS_TYPE_READ, ACCESS_MODE_DEBUG);
}

template <typename DT>
void
bitfield<DT>::
set_debug_write_callback(bitfield_debug_write_callback_base<DT>* cb)
{
  assert(cb);
  scoped_reference<bitfield_debug_write_callback_base<DT> > scopedRef(cb);
  if (has_default_debug_write_behavior()) {
    mRegister.bitfield_debug_write_callback_registered();
  }
  do_set_debug_write_callback(cb);
  mCallbackObservers.notify_callback_changed(ACCESS_TYPE_WRITE, ACCESS_MODE_DEBUG);
}

template <typename DT>
void
bitfield<DT>::
remove_debug_callback()
{
  remove_debug_read_callback();
  remove_debug_write_callback();
}

template <typename DT>
void
bitfield<DT>::
remove_debug_read_callback()
{
  if (!has_default_debug_read_behavior()) {
    scoped_reference<bitfield_debug_read_callback_base<DT> > scopedRef(mDefaultDebugReadCallback);
    do_set_debug_read_callback(mDefaultDebugReadCallback);
    mRegister.bitfield_debug_read_callback_unregistered();
    mCallbackObservers.notify_callback_changed(ACCESS_TYPE_READ, ACCESS_MODE_DEBUG);
  }
}

template <typename DT>
void
bitfield<DT>::
remove_debug_write_callback()
{
  if (!has_default_debug_write_behavior()) {
    scoped_reference<bitfield_debug_write_callback_base<DT> > scopedRef(mDefaultDebugWriteCallback);
    do_set_debug_write_callback(mDefaultDebugWriteCallback);
    mRegister.bitfield_debug_write_callback_unregistered();
    mCallbackObservers.notify_callback_changed(ACCESS_TYPE_WRITE, ACCESS_MODE_DEBUG);
  }
}

template <typename DT>
bitfield_debug_read_callback_base<DT>*
bitfield<DT>::
get_debug_read_callback() const
{
  return mDebugReadCallback;
}

template <typename DT>
bitfield_debug_write_callback_base<DT>*
bitfield<DT>::
get_debug_write_callback() const
{
  return mDebugWriteCallback;
}

template <typename DT>
bool
bitfield<DT>::
register_callback_observer(bitfield_callback_observer<DT>* o)
{
  return mCallbackObservers.register_observer(o);
}

template <typename DT>
bool
bitfield<DT>::
unregister_all_callback_observers()
{
  return mCallbackObservers.unregister_all_observers();
}

template <typename DT>
bool
bitfield<DT>::
unregister_callback_observer(bitfield_callback_observer<DT>* o)
{
  return mCallbackObservers.unregister_observer(o);
}

template <typename DT>
const char*
bitfield<DT>::
kind() const
{
  return "scml2::bitfield";
}

template <typename DT>
bool
bitfield<DT>::
filter(bool readNotWrite, DT& data, DT& bitMask) const
{
  bitfield_access_restriction_base<DT>* restriction = readNotWrite ? 
    get_read_restriction() : get_write_restriction();
  DT bfBitMask = extract_bits<DT>(bitMask, get_size(),get_offset());
  if ((get_bit_mask<DT>(get_size()) & bfBitMask) != 0) {
    if (!restriction->has_default_restriction()) {
      DT bfData = extract_bits<DT>(data, get_size(),get_offset());
      DT orig_data = bfData;
      DT orig_bit_enables = bfBitMask;
      bool error = restriction->restrict(bfData, bfBitMask);
      DT changed_bit_enables = orig_bit_enables & (~bfBitMask);
      bfData = (orig_data & ~changed_bit_enables) | (bfData & changed_bit_enables);
      data = insert_bits<DT>(data, bfData, get_size(), get_offset());
      bitMask = insert_bits<DT>(bitMask, bfBitMask, get_size(), get_offset());
      if (error) {
	return true;
      }
    }
    if (mBitfieldContainer.filter(readNotWrite, data, bitMask)) {
      return true;
    }
  }

  return false;
}

template <typename DT>
bool 
bitfield<DT>::
is_dmi_read_allowed() const
{
  return has_default_read_restriction() && mBitfieldContainer.is_dmi_read_allowed();
}

template <typename DT>
bool 
bitfield<DT>::
is_dmi_write_allowed() const
{
  return has_default_write_restriction() && mBitfieldContainer.is_dmi_write_allowed();
}


#ifdef _WIN32
#define SCML2_INSTANTIATE_TEMPLATE(type) template class SNPS_VP_API bitfield<type >;
#else
#define SCML2_INSTANTIATE_TEMPLATE(type) template class bitfield<type >;
#endif  
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}
