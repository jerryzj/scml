/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_READ_ADAPTER_TAGGED_H
#define SCML2_BITFIELD_READ_ADAPTER_TAGGED_H

#include <tlm.h>
#include <cassert>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT, typename C>
class bitfield_read_adapter_tagged
{
public:
  typedef bool (C::*CallbackType)(DT& value,
                                  sc_core::sc_time& t,
                                  int id);

public:
  bitfield_read_adapter_tagged(C& c, CallbackType cb, int id);

  bool operator()(DT& value, const DT& bitMask, sc_core::sc_time& t);

private:
  C& mC;
  CallbackType mCallback;
  int mId;
};

template <typename DT, typename C>
inline
bitfield_read_adapter_tagged<DT, C>
create_bitfield_read_adapter(C& c,
                             typename bitfield_read_adapter_tagged<DT, C>::CallbackType cb,
                             int id)
{
  return bitfield_read_adapter_tagged<DT, C>(c, cb, id);
}

template <typename DT, typename C>
inline
bitfield_read_adapter_tagged<DT, C>::
bitfield_read_adapter_tagged(C& c, CallbackType cb, int id) :
  mC(c),
  mCallback(cb),
  mId(id)
{
}

template <typename DT, typename C>
inline
bool
bitfield_read_adapter_tagged<DT, C>::
operator()(DT& value, const DT&, sc_core::sc_time& t)
{
  assert(mCallback);
  return (mC.*mCallback)(value, t, mId);
}

}

#endif
