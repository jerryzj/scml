/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_POST_WRITE_ADAPTER_TAGGED_H
#define SCML2_BITFIELD_POST_WRITE_ADAPTER_TAGGED_H

#include <scml2/bitfield.h>

#include <tlm.h>
#include <cassert>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT, typename C>
class bitfield_post_write_adapter_tagged
{
public:
  typedef void (C::*CallbackType)(int id);

public:
  bitfield_post_write_adapter_tagged(C& c, CallbackType cb, int id);

  bool operator()(const DT& value, const DT& bitMask, sc_core::sc_time& t);

private:
  C& mC;
  CallbackType mCallback;
  int mId;
};

template <typename DT, typename C>
inline
bitfield_post_write_adapter_tagged<DT, C>
create_bitfield_post_write_adapter(C& c,
                                   typename bitfield_post_write_adapter_tagged<DT, C>::CallbackType cb,
                                   int id)
{
  return bitfield_post_write_adapter_tagged<DT, C>(c, cb, id);
}

template <typename DT, typename C>
inline
bitfield_post_write_adapter_tagged<DT, C>::
bitfield_post_write_adapter_tagged(C& c, CallbackType cb, int id) :
  mC(c),
  mCallback(cb),
  mId(id)
{
}

template <typename DT, typename C>
inline
bool
bitfield_post_write_adapter_tagged<DT, C>::
operator()(const DT&, const DT&, sc_core::sc_time&)
{
  assert(mCallback);
  (mC.*mCallback)(mId);
  return true;
}

}

#endif
