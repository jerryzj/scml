/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_ROUTER_DEBUG_CALLBACK_H
#define SCML2_ROUTER_DEBUG_CALLBACK_H

#include <scml2/router_debug_callback_base.h>
#include <scml2/debug_callback.h>

#include <string>

namespace scml2 {

template <typename ADAPTER>
inline
router_debug_callback_base*
create_router_debug_callback(ADAPTER adapter, const std::string& name)
{
  return new debug_callback<ADAPTER, router_debug_callback_base>(adapter, name);
}

}

#endif
