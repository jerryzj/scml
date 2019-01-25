/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_SELFSYNC_READ_CALLBACK_H
#define SCML2_BITFIELD_SELFSYNC_READ_CALLBACK_H

#include <scml2/bitfield_read_callback_base.h>

#include <tlm.h>
#include <string>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT, typename ADAPTER>
class bitfield_selfsync_read_callback :
  public bitfield_read_callback_base<DT>
{
public:
  bitfield_selfsync_read_callback(ADAPTER adapter, const std::string& name);

  virtual bool read(DT& value, const DT& bitMask, sc_core::sc_time& t);
  virtual bool has_never_syncing_behavior() const;

protected:
  ADAPTER mAdapter;
};

template <typename DT, typename ADAPTER>
inline
bitfield_selfsync_read_callback<DT, ADAPTER>::
bitfield_selfsync_read_callback(ADAPTER adapter, const std::string& name) :
  bitfield_read_callback_base<DT>(name),
  mAdapter(adapter)
{
}

template <typename DT, typename ADAPTER>
inline
bool
bitfield_selfsync_read_callback<DT, ADAPTER>::
read(DT& value, const DT& bitMask, sc_core::sc_time& t)
{
  return mAdapter(value, bitMask, t);
}

template <typename DT, typename ADAPTER>
inline
bool
bitfield_selfsync_read_callback<DT, ADAPTER>::
has_never_syncing_behavior() const
{
  return false;
}

}

#endif
