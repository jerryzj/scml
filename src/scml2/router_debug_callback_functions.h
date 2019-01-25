/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_ROUTER_DEBUG_CALLBACK_FUNCTIONS_H
#define SCML2_ROUTER_DEBUG_CALLBACK_FUNCTIONS_H

#include <scml2/router_debug_callback.h>
#include <scml2/debug_transport_adapter.h>
#include <scml2/debug_transport_adapter_tagged.h>
#include <scml2/router.h>

#include <string>
#include <cassert>

namespace scml2 {

template <typename DT, typename C>
inline
void
set_debug_callback(router<DT>& routerObject,
                   C* c,
                   typename debug_transport_adapter<C>::CallbackType cb,
                   const std::string& cbName)
{
  assert(c);
  routerObject.set_debug_callback(
    create_router_debug_callback(
      create_debug_transport_adapter(*c, cb),
      cbName));
}

template <typename DT, typename C>
inline
void
set_debug_callback(router<DT>& routerObject,
                   C* c,
                   typename debug_transport_adapter_tagged<C>::CallbackType cb,
                   const std::string& cbName,
                   int id)
{
  assert(c);
  routerObject.set_debug_callback(
    create_router_debug_callback(
      create_debug_transport_adapter(*c, cb, id),
      cbName));
}

}

#endif
