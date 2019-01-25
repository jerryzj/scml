/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML_CALLBACK_OBSERVER_OWNER_H
#define SCML_CALLBACK_OBSERVER_OWNER_H

#include "scmlinc/scml_callback_observer.h"
#include "scmlinc/scml_types.h"
#include "scmlinc/scml_memory_pv_if.h"
#include <PV/PV.h>

namespace sc_core {
  class sc_time;
}

class scml_callback_observer_owner {
public:
  virtual ~scml_callback_observer_owner();

  #define DEFINE_HANDLE_CALLBACK(IF) \
    virtual void handle_begin_callback(scml_callback_observer* o, \
                                       const IF::request_type& req, \
                                       unsigned int id) = 0; \
    virtual void handle_end_callback(scml_callback_observer* o, \
                                     const IF::request_type& req, \
                                     unsigned int id) = 0;
  SCML_FOR_EACH_PV_INTERFACE_TYPE(DEFINE_HANDLE_CALLBACK)
  #undef DEFINE_HANDLE_CALLBACK
};

inline
scml_callback_observer_owner::
~scml_callback_observer_owner()
{
}

#endif
