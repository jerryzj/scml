/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#ifndef SCML2_BITFIELD_DEFAULT_DEBUG_READ_CALLBACK_H
#define SCML2_BITFIELD_DEFAULT_DEBUG_READ_CALLBACK_H

#include <scml2/bitfield_read_callback_base.h>
#include <scml2/bitfield.h>

namespace scml2 {

template <typename DT>
class bitfield_default_debug_read_callback :
  public bitfield_debug_read_callback_base<DT>
{
public:
  bitfield_default_debug_read_callback(bitfield<DT>& bitfield);

  virtual bool read(DT& value, const DT& bitMask);
  virtual bool has_default_behavior() const;

private:
  bitfield<DT>& mBitfield;
};

template <typename DT>
inline
bitfield_default_debug_read_callback<DT>::
bitfield_default_debug_read_callback(bitfield<DT>& bitfield) :
  bitfield_debug_read_callback_base<DT>(""),
  mBitfield(bitfield)
{
}

template <typename DT>
inline
bool
bitfield_default_debug_read_callback<DT>::
read(DT& value, const DT& bitMask)
{
  value = mBitfield.get_debug(bitMask);
  return true;
}

template <typename DT>
inline
bool
bitfield_default_debug_read_callback<DT>::
has_default_behavior() const
{
  return true;
}

}

#endif
