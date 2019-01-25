/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_POST_WRITE_OLD_NEW_ADAPTER_H
#define SCML2_BITFIELD_POST_WRITE_OLD_NEW_ADAPTER_H

#include <scml2/bitfield.h>

#include <tlm.h>
#include <cassert>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT, typename C>
class bitfield_post_write_old_new_adapter
{
public:
  typedef void (C::*CallbackType)(const DT& old_value, const DT& new_value, sc_core::sc_time& t);

public:
  bitfield_post_write_old_new_adapter(C& c, CallbackType cb);

  bool operator()(const DT& old_value, const DT& new_value, sc_core::sc_time& t);

private:
  C& mC;
  CallbackType mCallback;
};

template <typename DT, typename C>
inline
bitfield_post_write_old_new_adapter<DT, C>
create_bitfield_post_write_old_new_adapter(C& c,
                                   typename bitfield_post_write_old_new_adapter<DT, C>::CallbackType cb)
{
  return bitfield_post_write_old_new_adapter<DT, C>(c, cb);
}

template <typename DT, typename C>
inline
bitfield_post_write_old_new_adapter<DT, C>::
bitfield_post_write_old_new_adapter(C& c, CallbackType cb) :
  mC(c),
  mCallback(cb)
{
}

template <typename DT, typename C>
inline
bool
bitfield_post_write_old_new_adapter<DT, C>::
operator()(const DT& old_value, const DT& new_value, sc_core::sc_time& t)
{
  assert(mCallback);
  (mC.*mCallback)(old_value, new_value, t);
  return true;
}

}

#endif
