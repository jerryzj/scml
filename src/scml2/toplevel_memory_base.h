/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_TOPLEVEL_MEMORY_BASE_H
#define SCML2_TOPLEVEL_MEMORY_BASE_H

#include <scml2/memory_base.h>
#include <scml2/memory_slow_access_base.h>
#include <scml2/burst_unroller.h>
#include <scml2/debug_burst_unroller.h>
#include <scml2/mappable_if.h>
#include <scml2/streams.h>

#include <scml2/memory_dmi.h>
#include <scml2/memory_content_observers.h>
#include <scml2_logging/snps_vp_dll.h>
#include <scml2_logging/stream.h>
#include <scml2/tagged_message.h>
#include <tlm.h>
#include <systemc>
#include <set>

namespace tlm {
  class tlm_generic_payload;
  class tlm_dmi;
  class tlm_bw_direct_mem_if;
}

namespace scml2 {

template <typename DT> class memory_alias;
template <typename DT> class reg;
template <typename DT> class memory_index_reference;
class memory_storage_base;
class memory_content_observer;

class SNPS_VP_API toplevel_memory_base :
  public memory_base,
  public burst_unroller,
  public debug_burst_unroller,
  public mappable_if
{
  static bool s_old_style_dmi_latency;
public:
  static bool get_old_style_dmi_latency();
  static void set_old_style_dmi_latency(bool old_style);

  toplevel_memory_base(const std::string& name,
                       unsigned long long size,
                       unsigned int width);
  ~toplevel_memory_base();

  void set_default_read_latency(const sc_core::sc_time& t);
  const sc_core::sc_time& get_default_read_latency() const;
  void set_default_write_latency(const sc_core::sc_time& t);
  const sc_core::sc_time& get_default_write_latency() const;

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

  virtual std::string get_mapped_name() const;

  virtual void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
  virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data);
  virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);

  virtual void register_bw_direct_mem_if(tlm::tlm_bw_direct_mem_if* bwInterface);
  virtual void unregister_bw_direct_mem_if(tlm::tlm_bw_direct_mem_if* bwInterface);

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


  void fast_backdoor_changed();

  // sc_object
  virtual const char* kind() const;

protected:
  class filter_helper;

  bool already_filtered() {return mAlreadyFiltered;}
  void set_already_filtered(bool value) { mAlreadyFiltered=value;}
  void set_storage(memory_storage_base* storage, bool deleteInDestructor=true);
  virtual void invalidate_dmi();
  virtual void process_unrolled_transaction(tlm::tlm_generic_payload& trans,
                                            sc_core::sc_time& t);
  virtual unsigned int process_unrolled_debug_transaction(tlm::tlm_generic_payload& trans);

  template <typename DT> friend class memory_alias;
  template <typename DT> friend class reg;
  template <typename DT> friend class memory_index_reference;

  bool register_content_observer(memory_content_observer* o,
                                 unsigned long long offset);


  template <typename DT>
  memory_slow_access_base<DT>* create_slow_access(unsigned long long offset);
private:
  virtual unsigned int get_access_size_at_address(unsigned long long address) const;
  virtual unsigned int get_debug_access_size_at_address(unsigned long long address) const;
  void addDmiData(memory_base* alias);
  void addReadDmiData(memory_base* alias);
  void addWriteDmiData(memory_base* alias);

protected:
  streams* mTheStreams;
  memory_storage_base* mStorage;
  sc_core::sc_time mReadLatency;
  sc_core::sc_time mWriteLatency;
  memory_dmi mReadDmiData;
  memory_dmi mWriteDmiData;
  memory_content_observers mContentObservers;
  bool mDmiPointersInUse;
  std::set<tlm::tlm_bw_direct_mem_if*> mBwDirectMemIfs;
  bool mDelStorageOnDestruction;
  bool mAlreadyFiltered;
};

inline
void
toplevel_memory_base::
set_default_read_latency(const sc_core::sc_time& t)
{
  mReadLatency = t;
}

inline
const sc_core::sc_time&
toplevel_memory_base::
get_default_read_latency() const
{
  return mReadLatency;
}

inline
void
toplevel_memory_base::
set_default_write_latency(const sc_core::sc_time& t)
{
  mWriteLatency = t;
}

inline
const sc_core::sc_time&
toplevel_memory_base::
get_default_write_latency() const
{
  return mWriteLatency;
}

inline
tlm::tlm_response_status
toplevel_memory_base::
put_with_triggering_callbacks(unsigned long long address,
                              const unsigned char* data,
                              unsigned int dataLength,
                              const unsigned char* byteEnablePtr,
                              unsigned int byteEnableLength,
                              sc_core::sc_time& t)
{
  // FIXME: pool
  tlm::tlm_generic_payload trans;
  trans.set_address(address);
  trans.set_write();
  trans.set_data_ptr(const_cast<unsigned char*>(data));
  trans.set_data_length(dataLength);
  trans.set_byte_enable_ptr(const_cast<unsigned char*>(byteEnablePtr));
  trans.set_byte_enable_length(byteEnableLength);
  trans.set_streaming_width(dataLength);
  b_transport(trans, t);
  return trans.get_response_status();
}

inline
tlm::tlm_response_status
toplevel_memory_base::
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

inline
tlm::tlm_response_status
toplevel_memory_base::
get_with_triggering_callbacks(unsigned long long address,
                              unsigned char* data,
                              unsigned int dataLength,
                              const unsigned char* byteEnablePtr,
                              unsigned int byteEnableLength,
                              sc_core::sc_time& t)
{
  // FIXME: pool
  tlm::tlm_generic_payload trans;
  trans.set_address(address);
  trans.set_read();
  trans.set_data_ptr(data);
  trans.set_data_length(dataLength);
  trans.set_byte_enable_ptr(const_cast<unsigned char*>(byteEnablePtr));
  trans.set_byte_enable_length(byteEnableLength);
  trans.set_streaming_width(dataLength);
  b_transport(trans, t);
  return trans.get_response_status();
}

inline
tlm::tlm_response_status
toplevel_memory_base::
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

inline
unsigned int
toplevel_memory_base::
put_debug_with_triggering_callbacks(unsigned long long address,
                                    const unsigned char* data,
                                    unsigned int dataLength,
                                    const unsigned char* byteEnablePtr,
                                    unsigned int)
{
  if (byteEnablePtr) {
    SCML2_LOG_ERROR(this, get_streams().error(), SCML_INVALID_API_USAGE)
      << "byteEnablePtr should be 0x0 for"
      << " put_debug_with_triggering_callbacks on memory '"
      << get_name() << "'."
      << " TLM2 does not support byte enables for debug transactions"
      << std::endl;
  }

  // FIXME: pool
  tlm::tlm_generic_payload trans;
  trans.set_address(address);
  trans.set_write();
  trans.set_data_ptr(const_cast<unsigned char*>(data));
  trans.set_data_length(dataLength);
  trans.set_byte_enable_ptr(0x0);
  trans.set_streaming_width(dataLength);
  return transport_dbg(trans);
}

inline
unsigned int
toplevel_memory_base::
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

inline
unsigned int
toplevel_memory_base::
get_debug_with_triggering_callbacks(unsigned long long address,
                                    unsigned char* data,
                                    unsigned int dataLength,
                                    const unsigned char* byteEnablePtr,
                                    unsigned int)
{
  if (byteEnablePtr) {
    SCML2_LOG_ERROR(this, get_streams().error(), SCML_INVALID_API_USAGE)
      << "byteEnablePtr should be 0x0 for"
      << " get_debug_with_triggering_callbacks on memory '"
      << get_name() << "'."
      << " TLM2 does not support byte enables for debug transactions"
      << std::endl;
  }

  // FIXME: pool
  tlm::tlm_generic_payload trans;
  trans.set_address(address);
  trans.set_read();
  trans.set_data_ptr(data);
  trans.set_data_length(dataLength);
  trans.set_byte_enable_ptr(0x0);
  trans.set_streaming_width(dataLength);
  return transport_dbg(trans);
}

inline
unsigned int
toplevel_memory_base::
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

#define SCML2_INSTANTIATE_TEMPLATE(type) \
  SNPS_VP_API_TMPL_FUNC(memory_slow_access_base<type >* toplevel_memory_base::create_slow_access<type >(unsigned long long);)
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}
#endif
