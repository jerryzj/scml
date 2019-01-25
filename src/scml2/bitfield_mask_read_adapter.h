/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_MASK_READ_ADAPTER_H
#define SCML2_BITFIELD_MASK_READ_ADAPTER_H

#include <tlm.h>
#include <cassert>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT, typename C>
class bitfield_mask_read_adapter
{
public:
  typedef bool (C::*CallbackType)(DT& value,
                                  const DT& bitMask,
                                  sc_core::sc_time& t);

public:
  bitfield_mask_read_adapter(C& c, CallbackType cb);

  bool operator()(DT& value, const DT& bitMask, sc_core::sc_time& t);

private:
  C& mC;
  CallbackType mCallback;
};

template <typename DT, typename C>
inline
bitfield_mask_read_adapter<DT, C>
create_bitfield_read_adapter(C& c,
                             typename bitfield_mask_read_adapter<DT, C>::CallbackType cb)
{
  return bitfield_mask_read_adapter<DT, C>(c, cb);
}

template <typename DT, typename C>
inline
bitfield_mask_read_adapter<DT, C>::
bitfield_mask_read_adapter(C& c, CallbackType cb) :
  mC(c),
  mCallback(cb)
{
}

template <typename DT, typename C>
inline
bool
bitfield_mask_read_adapter<DT, C>::
operator()(DT& value, const DT& bitMask, sc_core::sc_time& t)
{
  assert(mCallback);
  return (mC.*mCallback)(value, bitMask, t);
}

}

#endif
