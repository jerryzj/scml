/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_PUT_CALLBACK_ADAPTER_H
#define SCML2_BITFIELD_PUT_CALLBACK_ADAPTER_H

#include <cassert>

namespace scml2 {

template <typename C, typename DT>
class bitfield_put_callback_adapter
{
public:
  typedef void (C::*CallbackType)(const DT& value);

public:
  bitfield_put_callback_adapter(C& c, CallbackType cb);

  void operator()(const DT& value);

private:
  C& mClass;
  CallbackType mCallback;
};

template <typename C, typename DT>
inline
bitfield_put_callback_adapter<C, DT>
create_bitfield_put_callback_adapter
(C& c, typename bitfield_put_callback_adapter<C, DT>::CallbackType cb)
{
  return bitfield_put_callback_adapter<C, DT>(c, cb);
}

template <typename C, typename DT>
inline
bitfield_put_callback_adapter<C, DT>::
bitfield_put_callback_adapter(C& c, CallbackType cb) 
  : mClass(c)
  , mCallback(cb)
{
}

template <typename C, typename DT>
inline
void
bitfield_put_callback_adapter<C, DT>::
operator()(const DT& value)
{
  assert(mCallback);
  (mClass.*mCallback)(value);
}

}

#endif
