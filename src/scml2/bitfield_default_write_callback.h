/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#ifndef SCML2_BITFIELD_DEFAULT_WRITE_CALLBACK_H
#define SCML2_BITFIELD_DEFAULT_WRITE_CALLBACK_H

#include <scml2/bitfield_write_callback_base.h>
#include <scml2/bitfield.h>

#include <tlm.h>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT>
class bitfield_default_write_callback :
  public bitfield_write_callback_base<DT>
{
public:
  bitfield_default_write_callback(bitfield<DT>& bitfield);

  virtual bool write(const DT& value, const DT& bitMask, sc_core::sc_time& t);
  virtual bool has_default_behavior() const;
  virtual bool has_never_syncing_behavior() const;

private:
  bitfield<DT>& mBitfield;
};

template <typename DT>
inline
bitfield_default_write_callback<DT>::
bitfield_default_write_callback(bitfield<DT>& bitfield) :
  bitfield_write_callback_base<DT>(""),
  mBitfield(bitfield)
{
}

template <typename DT>
inline
bool
bitfield_default_write_callback<DT>::
write(const DT& value, const DT& bitMask, sc_core::sc_time&)
{
  mBitfield.put(value, bitMask);
  return true;
}

template <typename DT>
inline
bool
bitfield_default_write_callback<DT>::
has_default_behavior() const
{
  return true;
}

template <typename DT>
inline
bool
bitfield_default_write_callback<DT>::
has_never_syncing_behavior() const
{
  return true;
}

}

#endif
