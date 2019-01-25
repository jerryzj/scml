/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_DEBUG_WRITE_CALLBACK_H
#define SCML2_BITFIELD_DEBUG_WRITE_CALLBACK_H

#include <scml2/bitfield_debug_write_callback_base.h>

#include <string>

namespace scml2 {

template <typename DT, typename ADAPTER>
class bitfield_debug_write_callback :
  public bitfield_debug_write_callback_base<DT>
{
public:
  bitfield_debug_write_callback(ADAPTER adapter, const std::string& name);

  virtual bool write(const DT& value, const DT& bitMask);

protected:
  ADAPTER mAdapter;
};

template <typename DT, typename ADAPTER>
inline
bitfield_debug_write_callback_base<DT>*
create_bitfield_debug_write_callback(ADAPTER adapter, const std::string& name)
{
  return new bitfield_debug_write_callback<DT, ADAPTER>(adapter, name);
}

template <typename DT, typename ADAPTER>
inline
bitfield_debug_write_callback<DT, ADAPTER>::
bitfield_debug_write_callback(ADAPTER adapter, const std::string& name) :
  bitfield_debug_write_callback_base<DT>(name),
  mAdapter(adapter)
{
}

template <typename DT, typename ADAPTER>
inline
bool
bitfield_debug_write_callback<DT, ADAPTER>::
write(const DT& value, const DT& bitMask)
{
  return mAdapter(value, bitMask);
}

}

#endif
