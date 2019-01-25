/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_NULL_ADAPTER_H
#define SCML2_NULL_ADAPTER_H

#include <tlm.h>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

class null_adapter
{
public:
  null_adapter() {
  }

  void operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time& /*t*/) {
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
  }
};

}

#endif
