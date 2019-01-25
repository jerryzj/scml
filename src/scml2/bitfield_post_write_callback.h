/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_POST_WRITE_CALLBACK_H
#define SCML2_BITFIELD_POST_WRITE_CALLBACK_H

#include <scml2/bitfield_neversync_post_write_callback.h>
#include <scml2/bitfield_autosync_post_write_callback.h>
#include <scml2/bitfield_selfsync_post_write_callback.h>
#include <scml2/bitfield_selfsync_post_write_callback.h>

#include <scml2/types.h>

#include <string>
#include <cassert>

namespace scml2 {

template <typename DT> class bitfield_post_write_callback_base;

template <typename DT, typename ADAPTER>
inline
bitfield_post_write_callback_base<DT>*
create_bitfield_post_write_callback(ADAPTER adapter,
                               const std::string& name,
                               SyncType syncType)
{
  switch (syncType) {
  case NEVER_SYNCING:
    return new bitfield_neversync_post_write_callback<DT, ADAPTER>(adapter, name);
  case AUTO_SYNCING:
    return new bitfield_autosync_post_write_callback<DT, ADAPTER>(adapter, name);
  case SELF_SYNCING:
    return new bitfield_selfsync_post_write_callback<DT, ADAPTER>(adapter, name);
  };
  assert(false);
  return 0;
}

}



#endif
