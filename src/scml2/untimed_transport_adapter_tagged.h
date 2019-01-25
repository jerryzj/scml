/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_UNTIMED_TRANSPORT_ADAPTER_TAGGED_H
#define SCML2_UNTIMED_TRANSPORT_ADAPTER_TAGGED_H

#include <cassert>

namespace tlm {
  class tlm_generic_payload;
}

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename C>
class untimed_transport_adapter_tagged
{
public:
  typedef void (C::*CallbackType)(tlm::tlm_generic_payload&, int id);

public:
  untimed_transport_adapter_tagged(C& c, CallbackType cb, int id);

  void operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);

private:
  C& mClass;
  CallbackType mCallback;
  int mId;
};

template <typename C>
inline
untimed_transport_adapter_tagged<C>
create_transport_adapter(C& c,
                         typename untimed_transport_adapter_tagged<C>::CallbackType cb,
                         int id)
{
  return untimed_transport_adapter_tagged<C>(c, cb, id);
}

template <typename C>
inline
untimed_transport_adapter_tagged<C>::
untimed_transport_adapter_tagged(C& c, CallbackType cb, int id) :
  mClass(c),
  mCallback(cb),
  mId(id)
{
}

template <typename C>
inline
void
untimed_transport_adapter_tagged<C>::
operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time&)
{
  assert(mCallback);
  (mClass.*mCallback)(trans, mId);
}

}

#endif
