/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_DISALLOW_DEBUG_ACCESS_CALLBACK_H
#define SCML2_DISALLOW_DEBUG_ACCESS_CALLBACK_H

#include <scml2/memory_debug_callback_base.h>
#include <scml2_logging/snps_vp_dll.h>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

class SNPS_VP_API memory_disallow_debug_access_callback : public memory_debug_callback_base
{
public:
  memory_disallow_debug_access_callback();

  virtual unsigned int execute(tlm::tlm_generic_payload& trans);
};

inline
memory_disallow_debug_access_callback::
memory_disallow_debug_access_callback() :
  memory_debug_callback_base("<disallow debug access callback>")
{
}

}

#endif
