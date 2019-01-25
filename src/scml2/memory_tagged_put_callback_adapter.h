/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_TAGGED_PUT_CALLBACK_ADAPTER_H
#define SCML2_MEMORY_TAGGED_PUT_CALLBACK_ADAPTER_H

#include <cassert>

namespace scml2 {

template <typename C, typename DT>
class memory_tagged_put_callback_adapter
{
public:
  typedef void (C::*CallbackType)(const DT& value, unsigned long long index, int id);

public:
  memory_tagged_put_callback_adapter(C& c, CallbackType cb, int id);

  void operator()(const DT& value, unsigned long long index);

private:
  C& mClass;
  CallbackType mCallback;
  int mId;
};

template <typename C, typename DT>
inline
memory_tagged_put_callback_adapter<C, DT>
create_memory_tagged_put_callback_adapter
(C& c, typename memory_tagged_put_callback_adapter<C, DT>::CallbackType cb, int id)
{
  return memory_tagged_put_callback_adapter<C, DT>(c, cb, id);
}

template <typename C, typename DT>
inline
memory_tagged_put_callback_adapter<C, DT>::
memory_tagged_put_callback_adapter(C& c, CallbackType cb, int id) 
  : mClass(c)
  , mCallback(cb)
  , mId(id)
{
}

template <typename C, typename DT>
inline
void
memory_tagged_put_callback_adapter<C, DT>::
operator()(const DT& value, unsigned long long index)
{
  assert(mCallback);
  (mClass.*mCallback)(value, index, mId);
}

}

#endif
