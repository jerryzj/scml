/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_ROUTER_NULL_DEBUG_CALLBACK_H
#define SCML2_ROUTER_NULL_DEBUG_CALLBACK_H

#include <scml2/router_debug_callback.h>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

class router_null_debug_callback : public router_debug_callback_base
{
public:
  router_null_debug_callback();

  virtual unsigned int execute(tlm::tlm_generic_payload& trans);
};

inline
router_null_debug_callback::
router_null_debug_callback()
{
}

inline
unsigned int
router_null_debug_callback::
execute(tlm::tlm_generic_payload&)
{
  return 0;
}

}

#endif
