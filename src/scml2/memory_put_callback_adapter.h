/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_PUT_CALLBACK_ADAPTER_H
#define SCML2_MEMORY_PUT_CALLBACK_ADAPTER_H

#include <cassert>

namespace scml2 {

template <typename C, typename DT>
class memory_put_callback_adapter
{
public:
  typedef void (C::*CallbackType)(const DT& value, unsigned long long index);

public:
  memory_put_callback_adapter(C& c, CallbackType cb);

  void operator()(const DT& value, unsigned long long index);

private:
  C& mClass;
  CallbackType mCallback;
};

template <typename C, typename DT>
inline
memory_put_callback_adapter<C, DT>
create_memory_put_callback_adapter
(C& c, typename memory_put_callback_adapter<C, DT>::CallbackType cb)
{
  return memory_put_callback_adapter<C, DT>(c, cb);
}

template <typename C, typename DT>
inline
memory_put_callback_adapter<C, DT>::
memory_put_callback_adapter(C& c, CallbackType cb) 
  : mClass(c)
  , mCallback(cb)
{
}

template <typename C, typename DT>
inline
void
memory_put_callback_adapter<C, DT>::
operator()(const DT& value, unsigned long long index)
{
  assert(mCallback);
  (mClass.*mCallback)(value, index);
}

}

#endif
