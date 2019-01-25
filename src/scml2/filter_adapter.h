/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_FILTER_ADAPTER_H
#define SCML2_FILTER_ADAPTER_H

#include <tlm.h>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename ADAPTER, typename C>
class filter_adapter
{
public:
  typedef bool (C::*CallbackType)(tlm::tlm_generic_payload&, sc_core::sc_time&);

  filter_adapter(ADAPTER f, C& c, CallbackType cb);

  void operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);

public:
  ADAPTER mAdapter;
  C& mClass;
  CallbackType mCallback;
};

template <typename C, typename ADAPTER>
inline
filter_adapter<ADAPTER, C>
create_filter_adapter(ADAPTER f, C& c, typename filter_adapter<ADAPTER, C>::CallbackType cb)
{
  return filter_adapter<ADAPTER, C>(f, c, cb);
}

template <typename ADAPTER, typename C>
inline
filter_adapter<ADAPTER, C>::
filter_adapter(ADAPTER f, C& c, CallbackType cb) :
  mAdapter(f),
  mClass(c),
  mCallback(cb)
{
}

template <typename ADAPTER, typename C>
inline
void
filter_adapter<ADAPTER, C>::
operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
  if ((mClass.*mCallback)(trans, t)) {
    mAdapter(trans, t);
  }
}

}

#endif
