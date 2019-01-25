/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_TLM2_GP_TARGET_ADPATER_H
#define SCML2_TLM2_GP_TARGET_ADPATER_H


#include <scml2/mappable_if.h>
#include <scml2/memory_select_callback.h>

#include <scml2_logging/snps_vp_dll.h>
#include <scml2_logging/stream.h>
#include <scml2_logging/severity.h>
#include <scml2/tagged_message.h>
#include <tlm.h>
#include <systemc>
#include <string>
#include <list>
#include <iostream>

namespace scml2 {

// Helper class to enable usage in DLLs (no usage of static member variables in templates)
class SNPS_VP_API tlm2_gp_target_adapter_base {
public:
  
protected:
  tlm2_gp_target_adapter_base() {}
  virtual ~tlm2_gp_target_adapter_base() {}
  virtual void nb2b_thread(tlm::tlm_generic_payload& trans) =0;
  
  class worker_thread {
  public:
    sc_core::sc_event mWakeup;
    tlm2_gp_target_adapter_base* mAdapter;
    tlm::tlm_generic_payload* mTrans;
    
    worker_thread();
    void single_shot_do_work();
  };
  
  static worker_thread* spawn_worker_thread();
};
 
template <unsigned int BUSWIDTH,
          int N = 1,
          sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND>
class tlm2_gp_target_adapter :
  public sc_core::sc_object,
  public tlm::tlm_fw_transport_if<>,
  public tlm::tlm_bw_direct_mem_if,
  public tlm2_gp_target_adapter_base
{
public:
  typedef tlm::tlm_base_target_socket<BUSWIDTH,
                                      tlm::tlm_fw_transport_if<>,
                                      tlm::tlm_bw_transport_if<>,
                                      N,
                                      POL> socket_type;

public:
  tlm2_gp_target_adapter(const std::string& name, socket_type& s);
  virtual ~tlm2_gp_target_adapter();

  std::string get_name() const;

  void operator()(mappable_if& destination);

  virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans,
                                             tlm::tlm_phase& phase,
                                             sc_core::sc_time& t);
  virtual void b_transport(tlm::tlm_generic_payload& trans,
                           sc_core::sc_time& t);
  virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans,
                                  tlm::tlm_dmi& dmiData);
  virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);

  virtual void invalidate_direct_mem_ptr(unsigned long long startRange,
                                         unsigned long long endRange);

  typedef tlm::tlm_fw_nonblocking_transport_if<tlm::tlm_generic_payload, tlm::tlm_phase> FwTransportIf;
  void register_fw_transport_if(FwTransportIf* fwInterface);
  void unregister_fw_transport_if();

  tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans,
                                     tlm::tlm_phase& phase,
                                     sc_core::sc_time& t);

  void set_select_callback(memory_select_callback_base* cb);

protected:
  virtual void nb2b_thread(tlm::tlm_generic_payload& trans);
  mappable_if* get_destination(tlm::tlm_generic_payload& trans) const;

protected:
  socket_type& mSocket;
  mappable_if* mDestination;
  int mBoundDestinations;
  memory_select_callback_base* mSelectCallback;
  bool mResponseInProgress;
  sc_core::sc_event mEndResponse;
  FwTransportIf* mFwTransportIf;
  mutable logging::stream mInternalError;
  mutable logging::stream mError;
};


template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
tlm2_gp_target_adapter(const std::string& name, socket_type& s) :
  sc_core::sc_object(name.c_str()),
  mSocket(s),
  mDestination(0),
  mBoundDestinations(0),
  mSelectCallback(0),
  mResponseInProgress(false),
  mFwTransportIf(0),
  mInternalError(this->name(), logging::severity::internal_error()),
  mError(this->name(), logging::severity::error())

{
  mSocket(*this);
}

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
~tlm2_gp_target_adapter()
{
  if (mSelectCallback) {
    mSelectCallback->unref();
  }
}

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
std::string
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
get_name() const
{
  return name();
}

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
void
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
operator()(mappable_if& destination)
{
  if (!mDestination) {
    // The first bound destination is the default destination
    mDestination = &destination;
  }
  mDestination->register_bw_direct_mem_if(this);
  mBoundDestinations++;
}

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
tlm::tlm_sync_enum
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
nb_transport_fw(tlm::tlm_generic_payload& trans,
                tlm::tlm_phase& phase,
                sc_core::sc_time& t)
{
  if (mFwTransportIf) {
    return mFwTransportIf->nb_transport_fw(trans, phase, t);

  } else {
    if (phase == tlm::BEGIN_REQ) {
      worker_thread* thread = spawn_worker_thread();
      thread->mAdapter = this;
      thread->mTrans = &trans;
      thread->mWakeup.notify(t);
      
      return tlm::TLM_ACCEPTED;

    } else if (phase == tlm::END_RESP) {
      mResponseInProgress = false;
      mEndResponse.notify(t);
      return tlm::TLM_COMPLETED;

    } else {
      return tlm::TLM_ACCEPTED;
    }
  }
}

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
void
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
nb2b_thread(tlm::tlm_generic_payload& trans)
{
  sc_core::sc_time t = sc_core::SC_ZERO_TIME;

  // forward call
  mappable_if* destination = get_destination(trans);
  if (!destination) {
    return;
  }

  (destination->b_transport)(trans, t);

  sc_core::wait(t);

  // return path
  while (mResponseInProgress) {
    sc_core::wait(mEndResponse);
  }
  t = sc_core::SC_ZERO_TIME;
  tlm::tlm_phase phase = tlm::BEGIN_RESP;
  tlm::tlm_sync_enum result = mSocket->nb_transport_bw(trans, phase, t);
  if (result == tlm::TLM_COMPLETED ||
      (result == tlm::TLM_UPDATED && phase == tlm::END_RESP)) {
    // transaction finished

  } else {
    mResponseInProgress = true;
  }
}


template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
mappable_if*
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
get_destination(tlm::tlm_generic_payload& payload) const
{
  if (mSelectCallback) {
    return mSelectCallback->execute(payload);
  }
  else {
    if (mBoundDestinations != 1) {
      if (mDestination) {
        SCML2_LOG(mError) << "Multiple interfaces bound, and no select callback registered" << std::endl;
      }
      else {
        SCML2_LOG(mError) << "no interface bound" << std::endl;
      }
    }
    return mDestination;
  }
}

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
void
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
set_select_callback(memory_select_callback_base* cb)
{
  if (mSelectCallback) {
    mSelectCallback->unref();
  }
  mSelectCallback = cb;
  mSelectCallback->ref();
}

template <typename T, typename C>
inline
void
set_select_callback(T& adapter, C* c, typename memory_select_callback<C>::CallbackType cb, const std::string& name)
{
  adapter.set_select_callback(new memory_select_callback<C>(*c, cb, name));
}

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
void
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
b_transport(tlm::tlm_generic_payload& trans,
            sc_core::sc_time& t)
{
  mappable_if* destination = get_destination(trans);
  if (destination) {
    destination->b_transport(trans, t);
  }
}

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
bool
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
get_direct_mem_ptr(tlm::tlm_generic_payload& trans,
                   tlm::tlm_dmi& dmiData)
{
  mappable_if* destination = get_destination(trans);
  if (destination) {
    return destination->get_direct_mem_ptr(trans, dmiData);
  }
  dmiData.set_start_address(0);
  dmiData.set_end_address(~0ULL);
  return false;
}

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
unsigned int
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
transport_dbg(tlm::tlm_generic_payload& trans)
{
  mappable_if* destination = get_destination(trans);
  if (destination) {
    return destination->transport_dbg(trans);
  }
  return 0;
}

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
void
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
invalidate_direct_mem_ptr(unsigned long long startRange,
                          unsigned long long endRange)
{
  mSocket->invalidate_direct_mem_ptr(startRange, endRange);
}

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
void
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
register_fw_transport_if(FwTransportIf* fwInterface)
{
  assert(fwInterface);
  mFwTransportIf = fwInterface;
}

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
void
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
unregister_fw_transport_if()
{
  assert(mFwTransportIf);
  mFwTransportIf = 0;
}

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline
tlm::tlm_sync_enum
tlm2_gp_target_adapter<BUSWIDTH, N, POL>::
nb_transport_bw(tlm::tlm_generic_payload& trans,
                tlm::tlm_phase& phase,
                sc_core::sc_time& t)
{
  return mSocket->nb_transport_bw(trans, phase, t);
}

class target_port_adaptor
{
protected:
  // Port adaptor is not constructed directly - use the create API instead
  target_port_adaptor() { }

  typedef tlm::tlm_fw_nonblocking_transport_if<tlm::tlm_generic_payload, tlm::tlm_phase> FwTransportIf;

public:
  template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
  static target_port_adaptor* create( const std::string& name,
                                      tlm::tlm_base_target_socket<BUSWIDTH,
                                            tlm::tlm_fw_transport_if<>,
                                            tlm::tlm_bw_transport_if<>,
                                            N,
                                            POL>* socket);

  // Provide non-template interface to templated tlm2_gp_target_adapter
  virtual std::string get_name() const = 0;
  virtual void operator()(mappable_if& destination) = 0;
  virtual void invalidate_direct_mem_ptr(unsigned long long startRange, unsigned long long endRange) = 0;
  virtual void register_fw_transport_if(FwTransportIf* fwInterface) = 0;
  virtual void unregister_fw_transport_if() = 0;
  virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& t) = 0;
  virtual void set_select_callback(memory_select_callback_base* cb) = 0;
};

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
class target_port_adaptor_impl : public target_port_adaptor
{
  typedef tlm2_gp_target_adapter<BUSWIDTH,N,POL> gp_adapter_type;

public:
  target_port_adaptor_impl( const std::string& name,
                            tlm::tlm_base_target_socket<BUSWIDTH,
                                    tlm::tlm_fw_transport_if<>,
                                    tlm::tlm_bw_transport_if<>,
                                    N,
                                    POL>* socket)
  {
    m_adapter = new gp_adapter_type(name, *socket);
  }

  virtual std::string get_name() const { return m_adapter->get_name(); }
  virtual void operator()(mappable_if& destination) { (*m_adapter)(destination); }
  virtual void invalidate_direct_mem_ptr(unsigned long long startRange, unsigned long long endRange) { m_adapter->invalidate_direct_mem_ptr(startRange, endRange); }
  virtual void register_fw_transport_if(target_port_adaptor::FwTransportIf* fwInterface) { m_adapter->register_fw_transport_if(fwInterface); }
  virtual void unregister_fw_transport_if() { m_adapter->unregister_fw_transport_if(); }
  virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& t) { return m_adapter->nb_transport_bw(trans, phase, t); }
  virtual void set_select_callback(memory_select_callback_base* cb) { m_adapter->set_select_callback(cb); }

private:
  gp_adapter_type* m_adapter;
};

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
target_port_adaptor* target_port_adaptor::create(
  const std::string& name,
  tlm::tlm_base_target_socket<BUSWIDTH,
                                    tlm::tlm_fw_transport_if<>,
                                    tlm::tlm_bw_transport_if<>,
                                    N,
                                    POL>* socket)
{
  return new target_port_adaptor_impl<BUSWIDTH,N,POL>(name, socket);
}


}

#endif
