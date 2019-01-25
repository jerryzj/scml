/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_AUTOSYNC_CALLBACK_H
#define SCML2_AUTOSYNC_CALLBACK_H

#include <systemc>
#include <string>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

template <typename ADAPTER, typename BASE>
class autosync_callback :
  public BASE
{
public:
  autosync_callback(ADAPTER adapter, const std::string& name);

  virtual void execute(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
  virtual bool has_never_syncing_behavior() const;

protected:
  ADAPTER mAdapter;
};

template <typename ADAPTER, typename BASE>
inline
autosync_callback<ADAPTER, BASE>::
autosync_callback(ADAPTER adapter, const std::string& name) :
  BASE(name),
  mAdapter(adapter)
{
}

template <typename ADAPTER, typename BASE>
inline
void
autosync_callback<ADAPTER, BASE>::
execute(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
  if (t != sc_core::SC_ZERO_TIME) {
    sc_core::wait(t);
    t = sc_core::SC_ZERO_TIME;
  }
  mAdapter(trans, t);
  sc_core::wait(sc_core::SC_ZERO_TIME);
}

template <typename ADAPTER, typename BASE>
inline
bool
autosync_callback<ADAPTER, BASE>::
has_never_syncing_behavior() const
{
  return false;
}

}

#endif
