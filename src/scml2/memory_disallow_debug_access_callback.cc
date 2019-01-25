/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#include <scml2/memory_disallow_debug_access_callback.h>

namespace scml2 {

unsigned int
memory_disallow_debug_access_callback::
execute(tlm::tlm_generic_payload&)
{
  return 0;
}

}
