/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_SELFSYNC_POST_WRITE_CALLBACK_H
#define SCML2_BITFIELD_SELFSYNC_POST_WRITE_CALLBACK_H

#include <scml2/bitfield_post_write_callback_base.h>

#include <tlm.h>
#include <string>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT, typename ADAPTER>
class bitfield_selfsync_post_write_callback :
  public bitfield_post_write_callback_base<DT>
{
public:
  bitfield_selfsync_post_write_callback(ADAPTER adapter, const std::string& name);

  virtual bool post_write(const DT& value, const DT& bitMask, sc_core::sc_time& t);
  virtual bool has_never_syncing_behavior() const;
  virtual bool has_default_behavior() const;


protected:
  ADAPTER mAdapter;
};

template <typename DT, typename ADAPTER>
inline
bitfield_selfsync_post_write_callback<DT, ADAPTER>::
bitfield_selfsync_post_write_callback(ADAPTER adapter, const std::string& name) :
  bitfield_post_write_callback_base<DT>(name),
  mAdapter(adapter)
{
}

template <typename DT, typename ADAPTER>
inline
bool
bitfield_selfsync_post_write_callback<DT, ADAPTER>::
has_default_behavior() const
{
  return true;
}


template <typename DT, typename ADAPTER>
inline
bool
bitfield_selfsync_post_write_callback<DT, ADAPTER>::
post_write(const DT& value, const DT& bitMask, sc_core::sc_time& t)
{
  return mAdapter(value, bitMask, t);
}

template <typename DT, typename ADAPTER>
inline
bool
bitfield_selfsync_post_write_callback<DT, ADAPTER>::
has_never_syncing_behavior() const
{
  return true;
}


}

#endif
