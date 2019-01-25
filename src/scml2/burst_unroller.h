/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BURST_UNROLLER_H
#define SCML2_BURST_UNROLLER_H

#include <scml2_logging/snps_vp_dll.h>

namespace tlm {
  class tlm_generic_payload;
}

namespace sc_core {
  class sc_time;
}

namespace scml2 {

class SNPS_VP_API burst_unroller
{
public:
  burst_unroller(unsigned int width);
  virtual ~burst_unroller();

protected:
  void unroll_transaction(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
  virtual void process_unrolled_transaction(tlm::tlm_generic_payload& trans, sc_core::sc_time& t) = 0;
  virtual unsigned int get_access_size_at_address(unsigned long long address) const;
private:
  unsigned int mWordSize;
};

}

#endif
