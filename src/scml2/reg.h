/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_REG_H
#define SCML2_REG_H

#include <scml2/memory_base.h>
#include <scml2/memory_fast_access.h>
#include <scml2/bitfield_container.h>

#include <scml2/memory_iterator.h>
#include <scml2/memory_const_iterator.h>

#include <scml2/memory_put_callback_owner.h>

#include <scml2/utils.h>

#include <scml2_logging/snps_vp_dll.h>

#include <tlm.h>
#include <vector>
#include <string>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT> class memory;
class memory_alias_base;
class bitfield_base;
template <typename DT> class bitfield;
class memory_content_observer;
class memory_callback_base;
class memory_debug_callback_base;

class SNPS_VP_API reg_base {};

// alias of size 1
// - no storage, toplevel memory has storage
// - must have parent (memory or memory_alias)
// - can have a transport/transportRead/transportWrite callback
// - can have a read/write callback
// - can NOT have other aliases or regs
// - can NOT bind to socket
template <typename DT>
class reg :
  public reg_base
  , public memory_base
  , public memory_put_callback_owner
{
public:
  typedef DT data_type;
  typedef memory_iterator<DT> iterator;
  typedef memory_const_iterator<DT> const_iterator;

public:
  reg(const std::string& name,
      toplevel_memory_base& parent,
      unsigned long long offset);
  reg(const std::string& name,
      memory_alias_base& parent,
      unsigned long long offset);
  ~reg();

  void initialize(const DT& value = DT());

  void put(unsigned long long address,
           const unsigned char* data,
           unsigned int dataLength,
           const unsigned char* byteEnablePtr,
           unsigned int byteEnableLength);
  void put(unsigned long long address,
           const unsigned char* data,
           unsigned int dataLength);
  void get(unsigned long long address,
           unsigned char* data,
           unsigned int dataLength,
           const unsigned char* byteEnablePtr,
           unsigned int byteEnableLength) const;
  void get(unsigned long long address,
           unsigned char* data,
           unsigned int dataLength) const;

  void put(const DT& data,
           unsigned int size,
           unsigned int offset);
  void put(const DT& data);
  DT get(unsigned int size,
         unsigned int offset) const;
  DT get() const;

  void put_debug(unsigned long long address,
                 const unsigned char* data,
                 unsigned int dataLength,
                 const unsigned char* byteEnablePtr,
                 unsigned int byteEnableLength);
  void put_debug(unsigned long long address,
                 const unsigned char* data,
                 unsigned int dataLength);
  void get_debug(unsigned long long address,
                 unsigned char* data,
                 unsigned int dataLength,
                 const unsigned char* byteEnablePtr,
                 unsigned int byteEnableLength) const;
  void get_debug(unsigned long long address,
                 unsigned char* data,
                 unsigned int dataLength) const;

  void put_debug(const DT& data,
                 unsigned int size,
                 unsigned int offset);
  void put_debug(const DT& data);
  DT get_debug(unsigned int size,
               unsigned int offset) const;
  DT get_debug() const;

  tlm::tlm_response_status
  put_with_triggering_callbacks(unsigned long long address,
                                const unsigned char* data,
                                unsigned int dataLength,
                                const unsigned char* byteEnablePtr,
                                unsigned int byteEnableLength,
                                sc_core::sc_time& t);
  tlm::tlm_response_status
  put_with_triggering_callbacks(unsigned long long address,
                                const unsigned char* data,
                                unsigned int dataLength,
                                sc_core::sc_time& t);
  tlm::tlm_response_status
  get_with_triggering_callbacks(unsigned long long address,
                                unsigned char* data,
                                unsigned int dataLength,
                                const unsigned char* byteEnablePtr,
                                unsigned int byteEnableLength,
                                sc_core::sc_time& t);
  tlm::tlm_response_status
  get_with_triggering_callbacks(unsigned long long address,
                                unsigned char* data,
                                unsigned int dataLength,
                                sc_core::sc_time& t);

  tlm::tlm_response_status
  put_with_triggering_callbacks(const DT& data,
                                unsigned int size,
                                unsigned int offset,
                                sc_core::sc_time& t);
  tlm::tlm_response_status
  put_with_triggering_callbacks(const DT& data,
                                sc_core::sc_time& t);
  tlm::tlm_response_status
  get_with_triggering_callbacks(DT& data,
                                unsigned int size,
                                unsigned int offset,
                                sc_core::sc_time& t);
  tlm::tlm_response_status
  get_with_triggering_callbacks(DT& data,
                                sc_core::sc_time& t);

  unsigned int
  put_debug_with_triggering_callbacks(unsigned long long address,
                                      const unsigned char* data,
                                      unsigned int dataLength,
                                      const unsigned char* byteEnablePtr,
                                      unsigned int byteEnableLength);
  unsigned int
  put_debug_with_triggering_callbacks(unsigned long long address,
                                      const unsigned char* data,
                                      unsigned int dataLength);
  unsigned int
  get_debug_with_triggering_callbacks(unsigned long long address,
                                      unsigned char* data,
                                      unsigned int dataLength,
                                      const unsigned char* byteEnablePtr,
                                      unsigned int byteEnableLength);
  unsigned int
  get_debug_with_triggering_callbacks(unsigned long long address,
                                      unsigned char* data,
                                      unsigned int dataLength);

  unsigned int
  put_debug_with_triggering_callbacks(const DT& data,
                                      unsigned int size,
                                      unsigned int offset);
  unsigned int
  put_debug_with_triggering_callbacks(const DT& data);
  unsigned int
  get_debug_with_triggering_callbacks(DT& data,
                                      unsigned int size,
                                      unsigned int offset);
  unsigned int
  get_debug_with_triggering_callbacks(DT& data);

  void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
  unsigned int transport_dbg(tlm::tlm_generic_payload& trans);

  void transport_without_triggering_callbacks(tlm::tlm_generic_payload& trans);
  void transport_debug_without_triggering_callbacks(tlm::tlm_generic_payload& trans);

  bool put_with_triggering_bitfield_callbacks(const DT& data, sc_core::sc_time& t);
  bool put_with_triggering_bitfield_callbacks(const DT& data, const DT& bitMask, sc_core::sc_time& t);
  bool get_with_triggering_bitfield_callbacks(DT& data, sc_core::sc_time& t);
  bool get_with_triggering_bitfield_callbacks(DT& data, const DT& bitMask, sc_core::sc_time& t);
  bool put_debug_with_triggering_bitfield_callbacks(const DT& data, const DT& bitMask);
  bool put_debug_with_triggering_bitfield_callbacks(const DT& data);
  bool get_debug_with_triggering_bitfield_callbacks(DT& data);
  bool get_debug_with_triggering_bitfield_callbacks(DT& data, const DT& bitMask);
  
  bool has_never_syncing_bitfield_read_behavior() const;
  bool has_never_syncing_bitfield_write_behavior() const;

  operator DT() const;

  reg& operator=(DT value);
  reg& operator =(const reg& r);

  reg& operator+=(DT value);
  reg& operator-=(DT value);
  reg& operator/=(DT value);
  reg& operator*=(DT value);
  reg& operator%=(DT value);
  reg& operator^=(DT value);
  reg& operator&=(DT value);
  reg& operator|=(DT value);
  reg& operator>>=(DT value);
  reg& operator<<=(DT value);

  reg& operator--();
  DT operator--(int);
  reg& operator++();
  DT operator++(int);

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  virtual void set_callback(memory_callback_base* cb);
  virtual void set_read_callback(memory_callback_base* cb);
  virtual void set_write_callback(memory_callback_base* cb);

  virtual void remove_callback();
  virtual void remove_read_callback();
  virtual void remove_write_callback();

  virtual void set_debug_callback(memory_debug_callback_base* cb);
  virtual void set_debug_read_callback(memory_debug_callback_base* cb);
  virtual void set_debug_write_callback(memory_debug_callback_base* cb);

  virtual void remove_debug_callback();
  virtual void remove_debug_read_callback();
  virtual void remove_debug_write_callback();

  // Debugger API
  virtual unsigned int read(unsigned long long address,
                            unsigned char* data,
                            unsigned int size);
  virtual unsigned int write(unsigned long long address,
                             const unsigned char* data,
                             unsigned int size);

  virtual bool register_content_observer(memory_content_observer* o);
  virtual bool unregister_all_content_observers();
  virtual bool unregister_content_observer(memory_content_observer* o);

  virtual unsigned int collect_bitfields(std::vector<bitfield_base*>& b) const;

  // sc_object
  virtual const char* kind() const;

  using memory_base::set_symbolic_name;
  bool set_symbolic_name(const DT& value, const std::string& name);
  using memory_base::get_symbolic_name;
  std::string get_symbolic_name(const DT& value);

protected:
  virtual bool is_dmi_read_allowed() const;
  virtual bool is_dmi_write_allowed() const;

  virtual void invalidate_dmi();
  virtual void update_fast_access();

  virtual void post_filter(tlm::tlm_generic_payload& trans) const;
private:
  friend class bitfield<DT>;

  void setup();

  void register_bitfield(bitfield<DT>& bitfield);

  void bitfield_read_callback_registered();
  void bitfield_write_callback_registered();
  void bitfield_post_write_callback_registered();
  void bitfield_read_callback_unregistered();
  void bitfield_write_callback_unregistered();
  void bitfield_post_write_callback_unregistered();

  void bitfield_debug_read_callback_registered();
  void bitfield_debug_write_callback_registered();
  void bitfield_debug_read_callback_unregistered();
  void bitfield_debug_write_callback_unregistered();

private:
  reg(const reg&);

private:
  toplevel_memory_base& mToplevel;
  memory_fast_access<DT> mFastAccess;

  bitfield_container<DT> mBitfieldContainer;
  bool mHasUserReadCallback;
  bool mHasUserWriteCallback;
  bool mHasUserDebugReadCallback;
  bool mHasUserDebugWriteCallback;
  bool mHasDefaultBitfieldReadBehavior;
  bool mHasDefaultBitfieldWriteBehavior;
  bool mHasDefaultBitfieldDebugReadBehavior;
  bool mHasDefaultBitfieldDebugWriteBehavior;
  bool mHasBitfieldPostWriteBehavior;
};

template <typename DT>
inline
void
reg<DT>::
put(unsigned long long address,
    const unsigned char* data,
    unsigned int dataLength,
    const unsigned char* byteEnablePtr,
    unsigned int byteEnableLength)
{
  mFastAccess.put(address,
                  data,
                  dataLength,
                  byteEnablePtr,
                  byteEnableLength,
                  false);
}

template <typename DT>
inline
void
reg<DT>::
put(unsigned long long address,
    const unsigned char* data,
    unsigned int dataLength)
{
  put(address, data, dataLength, 0, 0);
}

template <typename DT>
inline
void
reg<DT>::
get(unsigned long long address,
    unsigned char* data,
    unsigned int dataLength,
    const unsigned char* byteEnablePtr,
    unsigned int byteEnableLength) const
{
  mFastAccess.get(address,
                  data,
                  dataLength,
                  byteEnablePtr,
                  byteEnableLength,
                  false);
}

template <typename DT>
inline
void
reg<DT>::
get(unsigned long long address,
    unsigned char* data,
    unsigned int dataLength) const
{
  get(address, data, dataLength, 0, 0);
}


template <typename DT>
inline
void
reg<DT>::
put(const DT& data,
    unsigned int size,
    unsigned int offset)
{
  mFastAccess.put(0, data, size, offset, false);
}

template <typename DT>
inline
void
reg<DT>::
put(const DT& data)
{
  mFastAccess.put(0, data, false);
}

template <typename DT>
inline
DT
reg<DT>::
get(unsigned int size,
    unsigned int offset) const
{
  return mFastAccess.get(0, size, offset, false);
}

template <typename DT>
inline
DT
reg<DT>::
get() const
{
  return mFastAccess.get(0, false);
}

template <typename DT>
inline
void
reg<DT>::
put_debug(unsigned long long address,
          const unsigned char* data,
          unsigned int dataLength,
          const unsigned char* byteEnablePtr,
          unsigned int byteEnableLength)
{
  mFastAccess.put(address,
                  data,
                  dataLength,
                  byteEnablePtr,
                  byteEnableLength,
                  true);
}

template <typename DT>
inline
void
reg<DT>::
put_debug(unsigned long long address,
          const unsigned char* data,
          unsigned int dataLength)
{
  put_debug(address, data, dataLength, 0, 0);
}

template <typename DT>
inline
void
reg<DT>::
get_debug(unsigned long long address,
          unsigned char* data,
          unsigned int dataLength,
          const unsigned char* byteEnablePtr,
          unsigned int byteEnableLength) const
{
  mFastAccess.get(address,
                  data,
                  dataLength,
                  byteEnablePtr,
                  byteEnableLength,
                  true);
}

template <typename DT>
inline
void
reg<DT>::
get_debug(unsigned long long address,
          unsigned char* data,
          unsigned int dataLength) const
{
  get_debug(address, data, dataLength, 0, 0);
}


template <typename DT>
inline
void
reg<DT>::
put_debug(const DT& data,
          unsigned int size,
          unsigned int offset)
{
  mFastAccess.put(0, data, size, offset, true);
}

template <typename DT>
inline
void
reg<DT>::
put_debug(const DT& data)
{
  mFastAccess.put(0, data, true);
}

template <typename DT>
inline
DT
reg<DT>::
get_debug(unsigned int size,
          unsigned int offset) const
{
  return mFastAccess.get(0, size, offset, true);
}

template <typename DT>
inline
DT
reg<DT>::
get_debug() const
{
  return mFastAccess.get(0, true);
}

template <typename DT>
inline
tlm::tlm_response_status
reg<DT>::
put_with_triggering_callbacks(unsigned long long address,
                              const unsigned char* data,
                              unsigned int dataLength,
                              const unsigned char* byteEnablePtr,
                              unsigned int byteEnableLength,
                              sc_core::sc_time& t)
{
  return mToplevel.put_with_triggering_callbacks(address + mToplevel.get_width()*get_offset(),
                                                 data,
                                                 dataLength,
                                                 byteEnablePtr,
                                                 byteEnableLength,
                                                 t);
}

template <typename DT>
inline
tlm::tlm_response_status
reg<DT>::
put_with_triggering_callbacks(unsigned long long address,
                              const unsigned char* data,
                              unsigned int dataLength,
                              sc_core::sc_time& t)
{
  return put_with_triggering_callbacks(address,
                                       data,
                                       dataLength,
                                       0,
                                       0,
                                       t);
}

template <typename DT>
inline
tlm::tlm_response_status
reg<DT>::
get_with_triggering_callbacks(unsigned long long address,
                              unsigned char* data,
                              unsigned int dataLength,
                              const unsigned char* byteEnablePtr,
                              unsigned int byteEnableLength,
                              sc_core::sc_time& t)
{
  return mToplevel.get_with_triggering_callbacks(address + mToplevel.get_width()*get_offset(),
                                                 data,
                                                 dataLength,
                                                 byteEnablePtr,
                                                 byteEnableLength,
                                                 t);
}

template <typename DT>
inline
tlm::tlm_response_status
reg<DT>::
get_with_triggering_callbacks(unsigned long long address,
                              unsigned char* data,
                              unsigned int dataLength,
                              sc_core::sc_time& t)
{
  return get_with_triggering_callbacks(address,
                                       data,
                                       dataLength,
                                       0,
                                       0,
                                       t);
}

template <typename DT>
inline
tlm::tlm_response_status
reg<DT>::
put_with_triggering_callbacks(const DT& data,
                              unsigned int size,
                              unsigned int offset,
                              sc_core::sc_time& t)
{
  return mToplevel.put_with_triggering_callbacks(offset + mToplevel.get_width()*get_offset(),
                                                 dt_to_array<DT>(data),
                                                 size,
                                                 t);
}

template <typename DT>
inline
tlm::tlm_response_status
reg<DT>::
put_with_triggering_callbacks(const DT& data,
                              sc_core::sc_time& t)
{
  return put_with_triggering_callbacks(data,
                                       sizeOf<DT>(),
                                       0,
                                       t);
}

template <typename DT>
inline
tlm::tlm_response_status
reg<DT>::
get_with_triggering_callbacks(DT& data,
                              unsigned int size,
                              unsigned int offset,
                              sc_core::sc_time& t)
{
  dt_from_array<DT> x(data, size);
  return mToplevel.get_with_triggering_callbacks(offset + mToplevel.get_width()*get_offset(),
                                                 x,
                                                 size,
                                                 t);
}

template <typename DT>
inline
tlm::tlm_response_status
reg<DT>::
get_with_triggering_callbacks(DT& data,
                              sc_core::sc_time& t)
{
  return get_with_triggering_callbacks(data,
                                       sizeOf<DT>(),
                                       0,
                                       t);
}

template <typename DT>
inline
unsigned int
reg<DT>::
put_debug_with_triggering_callbacks(unsigned long long address,
                                    const unsigned char* data,
                                    unsigned int dataLength,
                                    const unsigned char* byteEnablePtr,
                                    unsigned int byteEnableLength)
{
  return mToplevel.put_debug_with_triggering_callbacks(address + mToplevel.get_width()*get_offset(),
                                                       data,
                                                       dataLength,
                                                       byteEnablePtr,
                                                       byteEnableLength);
}

template <typename DT>
inline
unsigned int
reg<DT>::
put_debug_with_triggering_callbacks(unsigned long long address,
                                    const unsigned char* data,
                                    unsigned int dataLength)
{
  return put_debug_with_triggering_callbacks(address,
                                             data,
                                             dataLength,
                                             0,
                                             0);
}

template <typename DT>
inline
unsigned int
reg<DT>::
get_debug_with_triggering_callbacks(unsigned long long address,
                                    unsigned char* data,
                                    unsigned int dataLength,
                                    const unsigned char* byteEnablePtr,
                                    unsigned int byteEnableLength)
{
  return mToplevel.get_debug_with_triggering_callbacks(address + mToplevel.get_width()*get_offset(),
                                                       data,
                                                       dataLength,
                                                       byteEnablePtr,
                                                       byteEnableLength);
}

template <typename DT>
inline
unsigned int
reg<DT>::
get_debug_with_triggering_callbacks(unsigned long long address,
                                    unsigned char* data,
                                    unsigned int dataLength)
{
  return get_debug_with_triggering_callbacks(address,
                                             data,
                                             dataLength,
                                             0,
                                             0);
}

template <typename DT>
inline
unsigned int
reg<DT>::
put_debug_with_triggering_callbacks(const DT& data,
                                    unsigned int size,
                                    unsigned int offset)
{
  return mToplevel.put_debug_with_triggering_callbacks(offset + mToplevel.get_width()*get_offset(),
                                                       dt_to_array<DT>(data),
                                                       size);
}

template <typename DT>
inline
unsigned int
reg<DT>::
put_debug_with_triggering_callbacks(const DT& data)
{
  return put_debug_with_triggering_callbacks(data,
                                             sizeOf<DT>(),
                                             0);
}

template <typename DT>
inline
unsigned int
reg<DT>::
get_debug_with_triggering_callbacks(DT& data,
                                    unsigned int size,
                                    unsigned int offset)
{
  dt_from_array<DT> x(data, size);
  return mToplevel.get_debug_with_triggering_callbacks(offset + mToplevel.get_width()*get_offset(),
                                                       x,
                                                       size);
}

template <typename DT>
inline
unsigned int
reg<DT>::
get_debug_with_triggering_callbacks(DT& data)
{
  return get_debug_with_triggering_callbacks(data,
                                             sizeOf<DT>(),
                                             0);
}

template <typename DT>
inline
void
reg<DT>::
b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
  const unsigned long long address = trans.get_address();
  trans.set_address(address + mToplevel.get_width()*get_offset());
  mToplevel.b_transport(trans, t);
  trans.set_address(address);
}

template <typename DT>
inline
unsigned int
reg<DT>::
transport_dbg(tlm::tlm_generic_payload& trans)
{
  const unsigned long long address = trans.get_address();
  trans.set_address(address + mToplevel.get_width()*get_offset());
  unsigned int result = mToplevel.transport_dbg(trans);
  trans.set_address(address);
  return result;
}

template <typename DT>
inline
void
reg<DT>::
transport_without_triggering_callbacks(tlm::tlm_generic_payload& trans)
{
  mFastAccess.transport(trans);
}

template <typename DT>
inline
void
reg<DT>::
transport_debug_without_triggering_callbacks(tlm::tlm_generic_payload& trans)
{
  mFastAccess.transport_debug(trans);
}

template <typename DT>
inline
bool
reg<DT>::
put_with_triggering_bitfield_callbacks(const DT& data, sc_core::sc_time& t)
{
  return put_with_triggering_bitfield_callbacks(data, get_bit_mask<DT>(get_width() * 8), t);
}

template <typename DT>
inline
bool
reg<DT>::
put_with_triggering_bitfield_callbacks(const DT& data, const DT& bitMask, sc_core::sc_time& t)
{
  const DT oldData = get_debug();
  
  if (mHasDefaultBitfieldWriteBehavior) {
    put((get_debug() & ~bitMask) | (data & bitMask));
  } else {
    // data passed to callback (writeData) should also contain data for disabled bits
    // for backwards compatibility of callbacks without bitMask
    const DT writeData = (get_debug() & ~bitMask) | (data & bitMask);
    DT bitsWritten = 0;

    if (mBitfieldContainer.execute_write_behavior(writeData, bitMask, t, bitsWritten)) {
      const DT updatedData = get_debug();
      put((writeData & ~bitsWritten) | (updatedData & bitsWritten));

    } else {
      return false;
    }
  }
  
  if (mHasBitfieldPostWriteBehavior) {
    mBitfieldContainer.execute_post_write_behavior(oldData, bitMask, t);
  }
  
  return true;
}

template <typename DT>
inline
bool
reg<DT>::
get_with_triggering_bitfield_callbacks(DT& data, sc_core::sc_time& t)
{
  return get_with_triggering_bitfield_callbacks(data, get_bit_mask<DT>(get_width() * 8), t);
}

template <typename DT>
inline
bool
reg<DT>::
get_with_triggering_bitfield_callbacks(DT& data, const DT& bitMask, sc_core::sc_time& t)
{
  if (mHasDefaultBitfieldReadBehavior) {
    data = (get() & bitMask) | (data & ~bitMask);
    return true;

  } else {
    // disabled bits should not be changed
    DT bitsRead = ~bitMask;

    if (mBitfieldContainer.execute_read_behavior(data, bitMask, t, bitsRead)) {
      data = (get() & ~bitsRead) | (data & bitsRead);
      return true;

    } else {
      return false;
    }
  }
}

template <typename DT>
inline
bool
reg<DT>::
put_debug_with_triggering_bitfield_callbacks(const DT& data)
{
  return put_debug_with_triggering_bitfield_callbacks(data, get_bit_mask<DT>(get_width() * 8));
}

template <typename DT>
inline
bool
reg<DT>::
put_debug_with_triggering_bitfield_callbacks(const DT& data, const DT& bitMask)
{
  if (mHasDefaultBitfieldDebugWriteBehavior) {
    put_debug((get_debug() & ~bitMask) | (data & bitMask));
    return true;

  } else {
    // data passed to callback (writeData) should also contain data for disabled bits
    // for backwards compatibility of callbacks without bitMask
    const DT writeData = (get_debug() & ~bitMask) | (data & bitMask);
    DT bitsWritten = 0;

    if (mBitfieldContainer.execute_debug_write_behavior(writeData, bitMask, bitsWritten)) {
      const DT updatedData = get_debug();
      put_debug((writeData & ~bitsWritten) | (updatedData & bitsWritten));
      return true;

    } else {
      return false;
    }
  }
}

template <typename DT>
inline
bool
reg<DT>::
get_debug_with_triggering_bitfield_callbacks(DT& data)
{
  return get_debug_with_triggering_bitfield_callbacks(data, get_bit_mask<DT>(get_width() * 8));
}

template <typename DT>
inline
bool
reg<DT>::
get_debug_with_triggering_bitfield_callbacks(DT& data, const DT& bitMask)
{
  if (mHasDefaultBitfieldDebugReadBehavior) {
    data = (get_debug() & bitMask) | (data & ~bitMask);
    return true;

  } else {
    // disabled bits should not be changed
    DT bitsRead = ~bitMask;

    if (mBitfieldContainer.execute_debug_read_behavior(data, bitMask, bitsRead)) {
      data = (get_debug() & ~bitsRead) | (data & bitsRead);
      return true;

    } else {
      return false;
    }
  }
}

template <typename DT>
inline
bool
reg<DT>::
has_never_syncing_bitfield_read_behavior() const
{
  return mBitfieldContainer.has_never_syncing_read_behavior();
}

template <typename DT>
inline
bool
reg<DT>::
has_never_syncing_bitfield_write_behavior() const
{
  return mBitfieldContainer.has_never_syncing_write_behavior();
}

template <typename DT>
inline
reg<DT>::
operator DT() const
{
  return get();
}

template <typename DT>
inline
reg<DT>&
reg<DT>::
operator=(DT value)
{
  put(value);
  return *this;
}

template <typename DT>
inline
reg<DT>&
reg<DT>::
operator =(const reg& r)
{
  put(r);
  return *this;
}

template <typename DT>
inline
reg<DT>&
reg<DT>::
operator+=(DT value)
{
  mFastAccess.doAdd(0, value);
  return *this;
}

template <typename DT>
inline
reg<DT>&
reg<DT>::
operator-=(DT value)
{
  mFastAccess.doSubtract(0, value);
  return *this;
}

template <typename DT>
inline
reg<DT>&
reg<DT>::
operator/=(DT value)
{
  mFastAccess.doDivide(0, value);
  return *this;
}

template <typename DT>
inline
reg<DT>&
reg<DT>::
operator*=(DT value)
{
  mFastAccess.doMultiply(0, value);
  return *this;
}

template <typename DT>
inline
reg<DT>&
reg<DT>::
operator%=(DT value)
{
  mFastAccess.doModulo(0, value);
  return *this;
}

template <typename DT>
inline
reg<DT>&
reg<DT>::
operator^=(DT value)
{
  mFastAccess.doXor(0, value);
  return *this;
}

template <typename DT>
inline
reg<DT>&
reg<DT>::
operator&=(DT value)
{
  mFastAccess.doAnd(0, value);
  return *this;
}

template <typename DT>
inline
reg<DT>&
reg<DT>::
operator|=(DT value)
{
  mFastAccess.doOr(0, value);
  return *this;
}

template <typename DT>
inline
reg<DT>&
reg<DT>::
operator>>=(DT value)
{
  mFastAccess.doShiftRight(0, value);
  return *this;
}

template <typename DT>
inline
reg<DT>&
reg<DT>::
operator<<=(DT value)
{
  mFastAccess.doShiftLeft(0, value);
  return *this;
}

template <typename DT>
inline
reg<DT>&
reg<DT>::
operator--()
{
  mFastAccess.doPreDecrement(0);
  return *this;
}

template <typename DT>
inline
DT
reg<DT>::
operator--(int)
{
  return mFastAccess.doPostDecrement(0);
}

template <typename DT>
inline
reg<DT>&
reg<DT>::
operator++()
{
  mFastAccess.doPreIncrement(0);
  return *this;
}

template <typename DT>
inline
DT
reg<DT>::
operator++(int)
{
  return mFastAccess.doPostIncrement(0);
}

template <typename DT>
inline
typename reg<DT>::iterator
reg<DT>::
begin()
{
  return iterator(&mToplevel, get_offset());
}

template <typename DT>
inline
typename reg<DT>::const_iterator
reg<DT>::
begin() const
{
  return const_iterator(&mToplevel, get_offset());
}

template <typename DT>
inline
typename reg<DT>::iterator
reg<DT>::
end()
{
  return begin() + 1;
}

template <typename DT>
inline
typename reg<DT>::const_iterator
reg<DT>::
end() const
{
  return begin() + 1;
}

template <typename DT>
inline
bool 
reg<DT>::
is_dmi_read_allowed() const
{
  return memory_base::is_dmi_read_allowed() && mBitfieldContainer.is_dmi_read_allowed();
}

template <typename DT>
inline
bool 
reg<DT>::
is_dmi_write_allowed() const
{
  return memory_base::is_dmi_write_allowed() && mBitfieldContainer.is_dmi_write_allowed();
}


#define SCML2_INSTANTIATE_TEMPLATE(type) \
   SNPS_VP_API_TMPL_CLASS(reg<type >;)
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}

#endif
