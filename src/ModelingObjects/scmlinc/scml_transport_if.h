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
 * Purpose of file : Definition of the Transport Interfaces supported by scml
 *
 */

#ifndef SCML_TRANSPORT_IF
#define SCML_TRANSPORT_IF

#include "scmlinc/scml_memory_pv_if.h"

class scml_transport_if : virtual public sc_core::sc_interface
  #define INHERIT_PV_INTERFACE(IF) , virtual public IF
  SCML_FOR_EACH_PV_INTERFACE_TYPE(INHERIT_PV_INTERFACE)
  #undef INHERIT_PV_INTERFACE
{
public:
  #define DEFINE_TRANSPORT(IF) \
    using IF::transport; \
    virtual IF::response_type transport(const IF::request_type& req) \
    { \
      assert(0); exit(1); \
      return req.obtainResp(); \
    }
  SCML_FOR_EACH_PV_INTERFACE_TYPE(DEFINE_TRANSPORT)
  #undef DEFINE_TRANSPORT
};

#endif
