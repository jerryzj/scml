/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_POST_WRITE_ADAPTER_H
#define SCML2_POST_WRITE_ADAPTER_H

#include <tlm.h>
#include <cassert>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename C>
class post_write_adapter
{
public:
  typedef void (C::*CallbackType)();

public:
  post_write_adapter(C& c, CallbackType cb);

  void operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);

private:
  C& mClass;
  CallbackType mCallback;
};

template <typename C>
inline
post_write_adapter<C>
create_post_write_adapter(C& c,
                          typename post_write_adapter<C>::CallbackType cb)
{
  return post_write_adapter<C>(c, cb);
}

template <typename C>
inline
post_write_adapter<C>::
post_write_adapter(C& c, CallbackType cb) :
  mClass(c),
  mCallback(cb)
{
}

template <typename C>
inline
void
post_write_adapter<C>::
operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time&)
{
  assert(trans.is_write());
  trans.set_response_status(tlm::TLM_OK_RESPONSE);
  assert(mCallback);
  (mClass.*mCallback)();
}

}

#endif
