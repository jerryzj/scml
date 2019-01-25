/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_H
#define SCML2_BITFIELD_H

#include <scml2/bitfield_base.h>
#include <scml2/reg.h>
#include <scml2/bitfield_read_callback_base.h>
#include <scml2/bitfield_write_callback_base.h>
#include <scml2/bitfield_post_write_callback_base.h>
#include <scml2/bitfield_debug_read_callback_base.h>
#include <scml2/bitfield_debug_write_callback_base.h>
#include <scml2/bitfield_callback_observers.h>
#include <scml2/bitfield_container.h>
#include <scml2/bitfield_access_restriction_base.h>
#include <scml2/restriction_owner.h>

#include <scml2/utils.h>
#include <scml2/scoped_reference.h>
#include <scml2/types.h>
#include <scml2_logging/snps_vp_dll.h>


#include <tlm.h>
#include <string>
#include <cassert>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT> class bitfield_callback_observer;

template <typename DT>
class bitfield 
  : public bitfield_base
  , public restriction_owner
{
public:
  typedef DT data_type;

public:
  bitfield(const std::string& name,
           reg<DT>& r,
           unsigned int offset,
           unsigned int size);
  bitfield(const std::string& name,
           bitfield<DT>& bf,
           unsigned int offset,
           unsigned int size);
  ~bitfield();

  void put(const DT& value);
  void put(const DT& value, const DT& bitMask);
  DT get() const;
  DT get(const DT& bitMask) const;

  void put_debug(const DT& value);
  void put_debug(const DT& value, const DT& bitMask);
  DT get_debug() const;
  DT get_debug(const DT& bitMask) const;

  bool put_with_triggering_callbacks(const DT& value,
                                     sc_core::sc_time& t);
  bool put_with_triggering_callbacks(const DT& value,
                                     const DT& bitMask,
                                     sc_core::sc_time& t);
  bool get_with_triggering_callbacks(DT& value,
                                     sc_core::sc_time& t) const;
  bool get_with_triggering_callbacks(DT& value,
                                     const DT& bitMask,
                                     sc_core::sc_time& t) const;

  bool put_debug_with_triggering_callbacks(const DT& value);
  bool put_debug_with_triggering_callbacks(const DT& value, const DT& bitMask);
  bool get_debug_with_triggering_callbacks(DT& value) const;
  bool get_debug_with_triggering_callbacks(DT& value, const DT& bitMask) const;

  operator DT() const;

  bitfield& operator=(DT value);
  bitfield& operator =(const bitfield& b);

  bitfield& operator+=(DT value);
  bitfield& operator-=(DT value);
  bitfield& operator/=(DT value);
  bitfield& operator*=(DT value);
  bitfield& operator%=(DT value);
  bitfield& operator^=(DT value);
  bitfield& operator&=(DT value);
  bitfield& operator|=(DT value);
  bitfield& operator<<=(DT value);
  bitfield& operator>>=(DT value);

  bitfield& operator--();
  DT operator--(int);
  bitfield& operator++();
  DT operator++(int);

  void set_read_callback(bitfield_read_callback_base<DT>* cb);
  void set_write_callback(bitfield_write_callback_base<DT>* cb);
  void set_post_write_callback(bitfield_post_write_callback_base<DT>* cb);
  void set_post_write_callback(DT enum_val, bitfield_post_write_callback_base<DT>* cb);
  void set_default_enum_write_callback(bitfield_post_write_callback_base<DT>* cb);


  void remove_callback();
  void remove_read_callback();
  void remove_write_callback();
  void remove_post_write_callback();

  bitfield_read_callback_base<DT>* get_read_callback() const;
  bitfield_write_callback_base<DT>* get_write_callback() const;

  void set_debug_read_callback(bitfield_debug_read_callback_base<DT>* cb);
  void set_debug_write_callback(bitfield_debug_write_callback_base<DT>* cb);

  void remove_debug_callback();
  void remove_debug_read_callback();
  void remove_debug_write_callback();

  void set_restriction(bitfield_access_restriction_base<DT>* r);
  void set_read_restriction(bitfield_access_restriction_base<DT>* r);
  void set_write_restriction(bitfield_access_restriction_base<DT>* r);

  bitfield_access_restriction_base<DT>* get_read_restriction() const;
  bitfield_access_restriction_base<DT>* get_write_restriction() const;

  void remove_restriction();
  void remove_read_restriction();
  void remove_write_restriction();

  bool has_default_read_restriction() const;
  bool has_default_write_restriction() const;

  bitfield_debug_read_callback_base<DT>* get_debug_read_callback() const;
  bitfield_debug_write_callback_base<DT>* get_debug_write_callback() const;

  bool has_default_read_behavior() const;
  bool has_default_write_behavior() const;
  bool has_default_debug_read_behavior() const;
  bool has_default_debug_write_behavior() const;
  bool has_never_syncing_read_behavior() const;
  bool has_never_syncing_write_behavior() const;
  bool has_post_write_behavior() const;

  bool register_callback_observer(bitfield_callback_observer<DT>* o);
  bool unregister_all_callback_observers();
  bool unregister_callback_observer(bitfield_callback_observer<DT>* o);

  // sc_object
  virtual const char* kind() const;

  using bitfield_base::set_symbolic_name;
  bool set_symbolic_name(const DT& value, const std::string& name);
  using bitfield_base::get_symbolic_name;
  std::string get_symbolic_name(const DT& value);

  const reg<DT>& get_register() const;
  reg<DT>& get_register();
  
  void store_old_value();
  DT get_old_value() const;

  bool filter(bool readNotWrite, DT& data, DT& bitMask) const;

  bool is_dmi_read_allowed() const;
  bool is_dmi_write_allowed() const;
private:
  virtual restriction_owner* get_restriction_parent() const;
  friend class bitfield_container<DT>;

  void setup();

  void register_bitfield(bitfield<DT>& bitfield);

  void do_set_read_callback(bitfield_read_callback_base<DT>* cb);
  void do_set_write_callback(bitfield_write_callback_base<DT>* cb);
  void do_set_post_write_callback(bitfield_post_write_callback_base<DT>* cb);
  void do_set_post_write_callback(DT enumval, bitfield_post_write_callback_base<DT>* cb);
  void do_set_default_enum_write_callback(bitfield_post_write_callback_base<DT>* cb);
  void do_set_debug_read_callback(bitfield_debug_read_callback_base<DT>* cb);
  void do_set_debug_write_callback(bitfield_debug_write_callback_base<DT>* cb);

  void do_set_read_restriction(bitfield_access_restriction_base<DT>* r);
  void do_set_write_restriction(bitfield_access_restriction_base<DT>* r);

  bool execute_write_behavior(const DT& value, const DT& bitMask, sc_core::sc_time& t);
  bool execute_read_behavior(DT& value, const DT& bitMask, sc_core::sc_time& t) const;
  bool execute_debug_write_behavior(const DT& value, const DT& bitMask);
  bool execute_debug_read_behavior(DT& value, const DT& bitMask) const;
  bool execute_post_write_behavior(const DT& value, const DT& oldValue, const DT& bitMask, sc_core::sc_time& t);
  

private:
  bitfield(const bitfield&);

private:
  reg<DT>& mRegister;
  bitfield<DT>* mParentBitfield;

  bitfield_read_callback_base<DT>* mDefaultReadCallback;
  bitfield_write_callback_base<DT>* mDefaultWriteCallback;
  bitfield_debug_read_callback_base<DT>* mDefaultDebugReadCallback;
  bitfield_debug_write_callback_base<DT>* mDefaultDebugWriteCallback;
  bitfield_post_write_callback_base<DT>* mDefaultPostWriteCallback;
  bitfield_read_callback_base<DT>* mReadCallback;
  bitfield_write_callback_base<DT>* mWriteCallback;
  bitfield_debug_read_callback_base<DT>* mDebugReadCallback;
  bitfield_debug_write_callback_base<DT>* mDebugWriteCallback;
  bitfield_post_write_callback_base<DT>* mPostWriteCallback;

  bitfield_callback_observers<DT> mCallbackObservers;

  bitfield_access_restriction_base<DT>* mDefaultRestriction;
  bitfield_access_restriction_base<DT>* mReadRestriction;
  bitfield_access_restriction_base<DT>* mWriteRestriction;

  bitfield_container<DT> mBitfieldContainer;
};

template <typename DT>
inline
void
bitfield<DT>::
put(const DT& value)
{
  DT data = insert_bits(mRegister.get_debug(), value, mSize, mOffset);
  mRegister.put(data);
}

template <typename DT>
inline
void
bitfield<DT>::
put(const DT& value, const DT& bitMask)
{
  const DT maskedValue = (get_debug() & ~bitMask) | (value & bitMask);
  DT data = insert_bits(mRegister.get_debug(), maskedValue, mSize, mOffset);
  mRegister.put(data);
}

template <typename DT>
inline
DT
bitfield<DT>::
get() const
{
  return extract_bits(mRegister.get(), mSize, mOffset);
}

template <typename DT>
inline
DT
bitfield<DT>::
get(const DT& bitMask) const
{
  return extract_bits(mRegister.get(), mSize, mOffset) & bitMask;
}

template <typename DT>
inline
void
bitfield<DT>::
put_debug(const DT& value)
{
  DT data = insert_bits(mRegister.get_debug(), value, mSize, mOffset);
  mRegister.put_debug(data);
}

template <typename DT>
inline
void
bitfield<DT>::
put_debug(const DT& value, const DT& bitMask)
{
  const DT maskedValue = (get_debug() & ~bitMask) | (value & bitMask);
  DT data = insert_bits(mRegister.get_debug(), maskedValue, mSize, mOffset);
  mRegister.put_debug(data);
}

template <typename DT>
inline
DT
bitfield<DT>::
get_debug() const
{
  return extract_bits(mRegister.get_debug(), mSize, mOffset);
}

template <typename DT>
inline
DT
bitfield<DT>::
get_debug(const DT& bitMask) const
{
  return extract_bits(mRegister.get_debug(), mSize, mOffset) & bitMask;
}

template <typename DT>
inline
bool
bitfield<DT>::
put_with_triggering_callbacks(const DT& value, sc_core::sc_time& t)
{
  return put_with_triggering_callbacks(value, get_bit_mask<DT>(mSize), t);
}

template <typename DT>
inline
bool
bitfield<DT>::
put_with_triggering_callbacks(const DT& value, const DT& bitMask, sc_core::sc_time& t)
{
  return mRegister.put_with_triggering_bitfield_callbacks(
           insert_bits<DT>(0, value, mSize, mOffset),
           insert_bits<DT>(0, bitMask, mSize, mOffset),
           t);
}

template <typename DT>
inline
bool
bitfield<DT>::
get_with_triggering_callbacks(DT& value, sc_core::sc_time& t) const
{
  return get_with_triggering_callbacks(value, get_bit_mask<DT>(mSize), t);
}

template <typename DT>
inline
bool
bitfield<DT>::
get_with_triggering_callbacks(DT& value, const DT& bitMask, sc_core::sc_time& t) const
{
  DT readData = insert_bits<DT>(0, value, mSize, mOffset);
  bool result = mRegister.get_with_triggering_bitfield_callbacks(
                  readData,
                  insert_bits<DT>(0, bitMask, mSize, mOffset),
                  t);
  value = extract_bits(readData, mSize, mOffset);
  return result;
}

template <typename DT>
inline
bool
bitfield<DT>::
put_debug_with_triggering_callbacks(const DT& value)
{
  return put_debug_with_triggering_callbacks(value, get_bit_mask<DT>(mSize));
}

template <typename DT>
inline
bool
bitfield<DT>::
put_debug_with_triggering_callbacks(const DT& value, const DT& bitMask)
{
  return mRegister.put_debug_with_triggering_bitfield_callbacks(
           insert_bits<DT>(0, value, mSize, mOffset),
           insert_bits<DT>(0, bitMask, mSize, mOffset));
}

template <typename DT>
inline
bool
bitfield<DT>::
get_debug_with_triggering_callbacks(DT& value) const
{
  return get_debug_with_triggering_callbacks(value, get_bit_mask<DT>(mSize));
}

template <typename DT>
inline
bool
bitfield<DT>::
get_debug_with_triggering_callbacks(DT& value, const DT& bitMask) const
{
  DT readData = insert_bits<DT>(0, value, mSize, mOffset);
  bool result = mRegister.get_debug_with_triggering_bitfield_callbacks(
                  readData,
                  insert_bits<DT>(0, bitMask, mSize, mOffset));
  value = extract_bits(readData, mSize, mOffset);
  return result;
}

template <typename DT>
inline
bool
bitfield<DT>::
execute_write_behavior(const DT& value, const DT& bitMask, sc_core::sc_time& t)
{
  assert(mWriteCallback);
  scoped_reference<bitfield_write_callback_base<DT> > scopedRef(mWriteCallback);
  unsigned int id =
    mCallbackObservers.notify_begin(&value, &bitMask, ACCESS_TYPE_WRITE, ACCESS_MODE_REGULAR);
  bool result = mWriteCallback->write(value, bitMask, t);
  mCallbackObservers.notify_end(0, id);
  return result;
}

template <typename DT>
inline
bool
bitfield<DT>::
execute_read_behavior(DT& value, const DT& bitMask, sc_core::sc_time& t) const
{
  assert(mReadCallback);
  scoped_reference<bitfield_read_callback_base<DT> > scopedRef(mReadCallback);
  unsigned int id =
    mCallbackObservers.notify_begin(0, &bitMask, ACCESS_TYPE_READ, ACCESS_MODE_REGULAR);
  bool result = mReadCallback->read(value, bitMask, t);
  mCallbackObservers.notify_end(&value, id);
  return result;
}

template <typename DT>
inline
bool
bitfield<DT>::
execute_debug_write_behavior(const DT& value, const DT& bitMask)
{
  assert(mDebugWriteCallback);
  scoped_reference<bitfield_debug_write_callback_base<DT> > scopedRef(mDebugWriteCallback);
  unsigned int id =
    mCallbackObservers.notify_begin(&value, &bitMask, ACCESS_TYPE_WRITE, ACCESS_MODE_DEBUG);
  bool result = mDebugWriteCallback->write(value, bitMask);
  mCallbackObservers.notify_end(0, id);
  return result;
}

template <typename DT>
inline
bool
bitfield<DT>::
execute_debug_read_behavior(DT& value, const DT& bitMask) const
{
  assert(mDebugReadCallback);
  scoped_reference<bitfield_debug_read_callback_base<DT> > scopedRef(mDebugReadCallback);
  unsigned int id =
    mCallbackObservers.notify_begin(0, &bitMask, ACCESS_TYPE_READ, ACCESS_MODE_DEBUG);
  bool result = mDebugReadCallback->read(value, bitMask);
  mCallbackObservers.notify_end(0, id);
  return result;
}

template <typename DT>
inline
bool
bitfield<DT>::
execute_post_write_behavior(const DT& oldValue, const DT& value, const DT& bitMask, sc_core::sc_time& t) 
{
  assert(mPostWriteCallback);
  scoped_reference<bitfield_post_write_callback_base<DT> > scopedRef(mPostWriteCallback);

  unsigned int id =
  mCallbackObservers.notify_begin(&value, &bitMask, ACCESS_TYPE_WRITE, ACCESS_MODE_REGULAR);
  bool result = mPostWriteCallback->post_write(oldValue, value, t);
  mCallbackObservers.notify_end(0, id);

  return result;
}

template <typename DT>
inline
bitfield<DT>::
operator DT() const
{
  return get();
}

template <typename DT>
inline
bitfield<DT>&
bitfield<DT>::
operator=(DT value)
{
  put(value);
  return *this;
}

template <typename DT>
inline
bitfield<DT>&
bitfield<DT>::
operator =(const bitfield& b)
{
  put(b);
  return *this;
}

template <typename DT>
inline
bitfield<DT>&
bitfield<DT>::
operator+=(DT value)
{
  put(get() + value);
  return *this;
}

template <typename DT>
inline
bitfield<DT>&
bitfield<DT>::
operator-=(DT value)
{
  put(get() - value);
  return *this;
}

template <typename DT>
inline
bitfield<DT>&
bitfield<DT>::
operator/=(DT value)
{
  put(get() / value);
  return *this;
}

template <typename DT>
inline
bitfield<DT>&
bitfield<DT>::
operator*=(DT value)
{
  put(get() * value);
  return *this;
}

template <typename DT>
inline
bitfield<DT>&
bitfield<DT>::
operator%=(DT value)
{
  put(get() % value);
  return *this;
}

template <typename DT>
inline
bitfield<DT>&
bitfield<DT>::
operator^=(DT value)
{
  put(get() ^ value);
  return *this;
}

template <typename DT>
inline
bitfield<DT>&
bitfield<DT>::
operator&=(DT value)
{
  put(get() & value);
  return *this;
}

template <typename DT>
inline
bitfield<DT>&
bitfield<DT>::
operator|=(DT value)
{
  put(get() | value);
  return *this;
}

template <typename DT>
inline
bitfield<DT>&
bitfield<DT>::
operator<<=(DT value)
{
  put(get() << value);
  return *this;
}

template <typename DT>
inline
bitfield<DT>&
bitfield<DT>::
operator>>=(DT value)
{
  put(get() >> value);
  return *this;
}

template <typename DT>
inline
bitfield<DT>&
bitfield<DT>::
operator--()
{
  put(get() - 1);
  return *this;
}

template <typename DT>
inline
DT
bitfield<DT>::
operator--(int)
{
  DT tmp = *this;
  --(*this);
  return tmp;
}

template <typename DT>
inline
bitfield<DT>&
bitfield<DT>::
operator++()
{
  put(get() + 1);
  return *this;
}

template <typename DT>
inline
DT
bitfield<DT>::
operator++(int)
{
  DT tmp = *this;
  ++(*this);
  return tmp;
}

template <typename DT>
inline
bool
bitfield<DT>::
has_default_read_behavior() const
{
  assert(mReadCallback);
  return mReadCallback->has_default_behavior();
}

template <typename DT>
inline
bool
bitfield<DT>::
has_default_write_behavior() const
{
  assert(mWriteCallback);
  return mWriteCallback->has_default_behavior();
}

template <typename DT>
inline
bool
bitfield<DT>::
has_default_read_restriction() const
{
  assert(mReadRestriction);
  return mReadRestriction->has_default_restriction();
}

template <typename DT>
inline
bool
bitfield<DT>::
has_default_write_restriction() const
{
  assert(mWriteRestriction);
  return mWriteRestriction->has_default_restriction();
}

template <typename DT>
inline
bool
bitfield<DT>::
has_default_debug_read_behavior() const
{
  assert(mDebugReadCallback);
  return mDebugReadCallback->has_default_behavior();
}

template <typename DT>
inline
bool
bitfield<DT>::
has_default_debug_write_behavior() const
{
  assert(mDebugWriteCallback);
  return mDebugWriteCallback->has_default_behavior();
}

template <typename DT>
inline
bool
bitfield<DT>::has_post_write_behavior() const
{
  assert(mPostWriteCallback);
  return mPostWriteCallback->has_default_behavior();
}

template <typename DT>
inline
bool
bitfield<DT>::
has_never_syncing_read_behavior() const
{
  assert(mReadCallback);
  return mReadCallback->has_never_syncing_behavior();
}

template <typename DT>
inline
bool
bitfield<DT>::
has_never_syncing_write_behavior() const
{
  assert(mWriteCallback);
  return mWriteCallback->has_never_syncing_behavior();
}

#define SCML2_INSTANTIATE_TEMPLATE(type)	\
   SNPS_VP_API_TMPL_CLASS(bitfield<type >;)
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}

#endif
