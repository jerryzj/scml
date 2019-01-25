/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_DEBUG_BURST_UNROLLER_H
#define SCML2_DEBUG_BURST_UNROLLER_H

#include <scml2_logging/snps_vp_dll.h>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

class SNPS_VP_API debug_burst_unroller
{
public:
  debug_burst_unroller(unsigned int width);
  virtual ~debug_burst_unroller();

protected:
  unsigned int unroll_debug_transaction(tlm::tlm_generic_payload& trans);
  virtual unsigned int process_unrolled_debug_transaction(tlm::tlm_generic_payload& trans) = 0;
  virtual unsigned int get_debug_access_size_at_address(unsigned long long address) const;
private:
  unsigned int mWordSize;
};

}

#endif
