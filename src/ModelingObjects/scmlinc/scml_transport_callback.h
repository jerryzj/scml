/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
/*
 * Modifications :
 *
 * Purpose of file : Definition of the Transport Callbacks supported by scml
 *
 */

#ifndef SCML_TRANSPORT_CALLBACK
#define SCML_TRANSPORT_CALLBACK

#include "scmlinc/scml_transport_if.h"
#include <systemc>

template <typename IF>
class scml_transport_callback : public scml_transport_if
{
public:
  typedef typename IF::response_type (sc_module::*TransportCallBack) (const typename IF::request_type &);

public:
  scml_transport_callback(sc_core::sc_module* mod, TransportCallBack cb)
    : mModule(mod),
      mCallBack(cb)
  {
  }

  virtual typename IF::response_type transport(const typename IF::request_type& req)
  {
    assert(mModule);
    assert(mCallBack);
    return (mModule->*mCallBack)(req);
  }
  using scml_transport_if::transport;

private:
  sc_core::sc_module* mModule;
  TransportCallBack mCallBack;
};

#endif

