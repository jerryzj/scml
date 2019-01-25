/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_ROUTER_CALLBACK_FUNCTIONS_H
#define SCML2_ROUTER_CALLBACK_FUNCTIONS_H

#include <scml2/router_callback.h>
#include <scml2/transport_adapter.h>
#include <scml2/transport_adapter_tagged.h>
#include <scml2/router.h>
#include <scml2/types.h>

#include <string>
#include <cassert>

namespace scml2 {

template <typename DT, typename C>
inline
void
set_callback(router<DT>& routerObject,
             C* c,
             typename transport_adapter<C>::CallbackType cb,
             const std::string& cbName,
             SyncType syncType = AUTO_SYNCING)
{
  assert(c);
  routerObject.set_callback(
    create_router_callback(
      create_transport_adapter(*c, cb),
      cbName,
      syncType));
}

template <typename DT, typename C>
inline
void
set_callback(router<DT>& routerObject,
             C* c,
             typename transport_adapter_tagged<C>::CallbackType cb,
             const std::string& cbName,
             SyncType syncType,
             int id)
{
  assert(c);
  routerObject.set_callback(
    create_router_callback(
      create_transport_adapter(*c, cb, id),
      cbName,
      syncType));
}

}

#endif
