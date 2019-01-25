/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_ALIAS_H
#define SCML2_MEMORY_ALIAS_H

#include <scml2/memory_base.h>
#include <scml2/memory_fast_access.h>

#include <scml2/memory_iterator.h>
#include <scml2/memory_const_iterator.h>
#include <scml2/memory_index_reference.h>

#include <scml2/memory_put_callback_owner.h>

#include <scml2/utils.h>
#include <scml2_logging/snps_vp_dll.h>

#include <tlm.h>
#include <string>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT> class memory;
template <typename DT> class reg;
template <typename DT> class memory_alias;

class memory_content_observer;

class memory_alias_base 
  : public memory_base
  , public memory_put_callback_owner
{
protected:
  memory_alias_base(toplevel_memory_base& memory,
		    const std::string& name,
		    memory_base* parent,
		    unsigned long long offset,
		    unsigned long long size,
		    unsigned int width);
  toplevel_memory_base& mToplevel;

  template <typename DT> friend class reg;
  template <typename DT> friend class memory_alias;
public:
  virtual ~memory_alias_base();
};

// alias:
// - no storage, toplevel memory has storage
// - must have parent (memory or memory_alias)
// - can have a transport/transportRead/transportWrite callback
// - can have other aliases or registers (alias of size 1)
// - can NOT bind to socket
template <typename DT>
class memory_alias :
  public memory_alias_base
{
public:
  typedef DT data_type;
  typedef memory_index_reference<DT> reference;
  typedef memory_iterator<DT> iterator;
  typedef memory_const_iterator<DT> const_iterator;

public:
  memory_alias(const std::string& name,
               toplevel_memory_base& parent,
               unsigned long long offset,
               unsigned long long size);
  memory_alias(const std::string& name,
               memory_alias_base& parent,
               unsigned long long offset,
               unsigned long long size);
  ~memory_alias();

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

  void put(unsigned long long index,
           const DT& data,
           unsigned int size,
           unsigned int offset);
  void put(unsigned long long index,
           const DT& data);
  DT get(unsigned long long index,
         unsigned int size,
         unsigned int offset) const;
  DT get(unsigned long long index) const;

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

  void put_debug(unsigned long long index,
                 const DT& data,
                 unsigned int size,
                 unsigned int offset);
  void put_debug(unsigned long long index,
                 const DT& data);
  DT get_debug(unsigned long long index,
               unsigned int size,
               unsigned int offset) const;
  DT get_debug(unsigned long long index) const;

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
  put_with_triggering_callbacks(unsigned long long index,
                                const DT& data,
                                unsigned int size,
                                unsigned int offset,
                                sc_core::sc_time& t);
  tlm::tlm_response_status
  put_with_triggering_callbacks(unsigned long long index,
                                const DT& data,
                                sc_core::sc_time& t);
  tlm::tlm_response_status
  get_with_triggering_callbacks(unsigned long long index,
                                DT& data,
                                unsigned int size,
                                unsigned int offset,
                                sc_core::sc_time& t);
  tlm::tlm_response_status
  get_with_triggering_callbacks(unsigned long long index,
                                DT& data,
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
  put_debug_with_triggering_callbacks(unsigned long long index,
                                      const DT& data,
                                      unsigned int size,
                                      unsigned int offset);
  unsigned int
  put_debug_with_triggering_callbacks(unsigned long long index,
                                      const DT& data);
  unsigned int
  get_debug_with_triggering_callbacks(unsigned long long index,
                                      DT& data,
                                      unsigned int size,
                                      unsigned int offset);
  unsigned int
  get_debug_with_triggering_callbacks(unsigned long long index,
                                      DT& data);

  void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
  unsigned int transport_dbg(tlm::tlm_generic_payload& trans);

  void transport_without_triggering_callbacks(tlm::tlm_generic_payload& trans);
  void transport_debug_without_triggering_callbacks(tlm::tlm_generic_payload& trans);

  reference operator[](unsigned long long index);
  DT operator[](unsigned long long index) const;

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

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


  // sc_object
  virtual const char* kind() const;

  using memory_base::set_symbolic_name;
  bool set_symbolic_name(const DT& value, const std::string& name);
  using memory_base::get_symbolic_name;
  std::string get_symbolic_name(const DT& value) const;

protected:
  virtual void invalidate_dmi();
  virtual void update_fast_access();

private:
  friend class reg<DT>;

  void setup();

  void check_size(unsigned long long size) const;
private:
  memory_alias(const memory_alias&);
  memory_alias& operator =(const memory_alias&);

private:
  memory_fast_access<DT> mFastAccess;
};

template <typename DT>
inline
void
memory_alias<DT>::
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
memory_alias<DT>::
put(unsigned long long address,
    const unsigned char* data,
    unsigned int dataLength)
{
  put(address, data, dataLength, 0, 0);
}

template <typename DT>
inline
void
memory_alias<DT>::
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
memory_alias<DT>::
get(unsigned long long address,
    unsigned char* data,
    unsigned int dataLength) const
{
  get(address, data, dataLength, 0, 0);
}


template <typename DT>
inline
void
memory_alias<DT>::
put(unsigned long long index,
    const DT& data,
    unsigned int size,
    unsigned int offset)
{
  mFastAccess.put(index, data, size, offset, false);
}

template <typename DT>
inline
void
memory_alias<DT>::
put(unsigned long long index,
    const DT& data)
{
  mFastAccess.put(index, data, false);
}

template <typename DT>
inline
DT
memory_alias<DT>::
get(unsigned long long index,
    unsigned int size,
    unsigned int offset) const
{
  return mFastAccess.get(index, size, offset, false);
}

template <typename DT>
inline
DT
memory_alias<DT>::
get(unsigned long long index) const
{
  return mFastAccess.get(index, false);
}

template <typename DT>
inline
void
memory_alias<DT>::
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
memory_alias<DT>::
put_debug(unsigned long long address,
          const unsigned char* data,
          unsigned int dataLength)
{
  put_debug(address, data, dataLength, 0, 0);
}

template <typename DT>
inline
void
memory_alias<DT>::
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
memory_alias<DT>::
get_debug(unsigned long long address,
          unsigned char* data,
          unsigned int dataLength) const
{
  get_debug(address, data, dataLength, 0, 0);
}


template <typename DT>
inline
void
memory_alias<DT>::
put_debug(unsigned long long index,
          const DT& data,
          unsigned int size,
          unsigned int offset)
{
  mFastAccess.put(index, data, size, offset, true);
}

template <typename DT>
inline
void
memory_alias<DT>::
put_debug(unsigned long long index,
          const DT& data)
{
  mFastAccess.put(index, data, true);
}

template <typename DT>
inline
DT
memory_alias<DT>::
get_debug(unsigned long long index,
          unsigned int size,
          unsigned int offset) const
{
  return mFastAccess.get(index, size, offset, true);
}

template <typename DT>
inline
DT
memory_alias<DT>::
get_debug(unsigned long long index) const
{
  return mFastAccess.get(index, true);
}

template <typename DT>
inline
tlm::tlm_response_status
memory_alias<DT>::
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
memory_alias<DT>::
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
memory_alias<DT>::
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
memory_alias<DT>::
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
memory_alias<DT>::
put_with_triggering_callbacks(unsigned long long index,
                              const DT& data,
                              unsigned int size,
                              unsigned int offset,
                              sc_core::sc_time& t)
{
  return mToplevel.put_with_triggering_callbacks(index_to_address<DT>(index)  + mToplevel.get_width()*get_offset() + offset,
                                                 dt_to_array<DT>(data),
                                                 size,
                                                 t);
}

template <typename DT>
inline
tlm::tlm_response_status
memory_alias<DT>::
put_with_triggering_callbacks(unsigned long long index,
                              const DT& data,
                              sc_core::sc_time& t)
{
  return put_with_triggering_callbacks(index,
                                       data,
                                       sizeOf<DT>(),
                                       0,
                                       t);
}

template <typename DT>
inline
tlm::tlm_response_status
memory_alias<DT>::
get_with_triggering_callbacks(unsigned long long index,
                              DT& data,
                              unsigned int size,
                              unsigned int offset,
                              sc_core::sc_time& t)
{
  dt_from_array<DT> x(data, size);
  return mToplevel.get_with_triggering_callbacks(index_to_address<DT>(index) + mToplevel.get_width()*get_offset() + offset,
                                                 x,
                                                 size,
                                                 t);
}

template <typename DT>
inline
tlm::tlm_response_status
memory_alias<DT>::
get_with_triggering_callbacks(unsigned long long index,
                              DT& data,
                              sc_core::sc_time& t)
{
  return get_with_triggering_callbacks(index,
                                       data,
                                       sizeOf<DT>(),
                                       0,
                                       t);
}

template <typename DT>
inline
unsigned int
memory_alias<DT>::
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
memory_alias<DT>::
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
memory_alias<DT>::
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
memory_alias<DT>::
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
memory_alias<DT>::
put_debug_with_triggering_callbacks(unsigned long long index,
                                    const DT& data,
                                    unsigned int size,
                                    unsigned int offset)
{
  return mToplevel.put_debug_with_triggering_callbacks(index_to_address<DT>(index) + mToplevel.get_width()*get_offset() + offset,
                                                       dt_to_array<DT>(data),
                                                       size);
}

template <typename DT>
inline
unsigned int
memory_alias<DT>::
put_debug_with_triggering_callbacks(unsigned long long index,
                                    const DT& data)
{
  return put_debug_with_triggering_callbacks(index,
                                             data,
                                             sizeOf<DT>(),
                                             0);
}

template <typename DT>
inline
unsigned int
memory_alias<DT>::
get_debug_with_triggering_callbacks(unsigned long long index,
                                    DT& data,
                                    unsigned int size,
                                    unsigned int offset)
{
  dt_from_array<DT> x(data, size);
  return mToplevel.get_debug_with_triggering_callbacks(index_to_address<DT>(index) + mToplevel.get_width()*get_offset() + offset,
                                                       x,
                                                       size);
}

template <typename DT>
inline
unsigned int
memory_alias<DT>::
get_debug_with_triggering_callbacks(unsigned long long index,
                                    DT& data)
{
  return get_debug_with_triggering_callbacks(index,
                                             data,
                                             sizeOf<DT>(),
                                             0);
}

template <typename DT>
inline
void
memory_alias<DT>::
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
memory_alias<DT>::
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
memory_alias<DT>::
transport_without_triggering_callbacks(tlm::tlm_generic_payload& trans)
{
  mFastAccess.transport(trans);
}

template <typename DT>
inline
void
memory_alias<DT>::
transport_debug_without_triggering_callbacks(tlm::tlm_generic_payload& trans)
{
  mFastAccess.transport_debug(trans);
}

template <typename DT>
inline
typename memory_alias<DT>::reference
memory_alias<DT>::
operator[](unsigned long long index)
{
//  return reference(&mToplevel, get_offset() + index);
  return reference(&mToplevel, get_offset(), index);
}

template <typename DT>
inline
DT
memory_alias<DT>::
operator[](unsigned long long index) const
{
  return get(index);
}

template <typename DT>
inline
typename memory_alias<DT>::iterator
memory_alias<DT>::
begin()
{
  return iterator(&mToplevel, get_offset());
}

template <typename DT>
inline
typename memory_alias<DT>::const_iterator
memory_alias<DT>::
begin() const
{
  return const_iterator(&mToplevel, get_offset());
}

template <typename DT>
inline
typename memory_alias<DT>::iterator
memory_alias<DT>::
end()
{
  return begin() + get_size();
}

template <typename DT>
inline
typename memory_alias<DT>::const_iterator
memory_alias<DT>::
end() const
{
  return begin() + get_size();
}

#define SCML2_INSTANTIATE_TEMPLATE(type) \
   SNPS_VP_API_TMPL_CLASS(memory_alias<type >;)
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}

#endif
