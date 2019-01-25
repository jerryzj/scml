/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_POST_WRITE_ADAPTER_TAGGED_H
#define SCML2_POST_WRITE_ADAPTER_TAGGED_H

#include <tlm.h>
#include <cassert>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename C>
class post_write_adapter_tagged
{
public:
  typedef void (C::*CallbackType)(int id);

public:
  post_write_adapter_tagged(C& c, CallbackType cb, int id);

  void operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);

private:
  C& mClass;
  CallbackType mCallback;
  int mId;
};

template <typename C>
inline
post_write_adapter_tagged<C>
create_post_write_adapter(C& c,
                          typename post_write_adapter_tagged<C>::CallbackType cb,
                          int id)
{
  return post_write_adapter_tagged<C>(c, cb, id);
}

template <typename C>
inline
post_write_adapter_tagged<C>::
post_write_adapter_tagged(C& c, CallbackType cb, int id) :
  mClass(c),
  mCallback(cb),
  mId(id)
{
}

template <typename C>
inline
void
post_write_adapter_tagged<C>::
operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time&)
{
  assert(trans.is_write());
  trans.set_response_status(tlm::TLM_OK_RESPONSE);
  assert(mCallback);
  (mClass.*mCallback)(mId);
}

}

#endif
