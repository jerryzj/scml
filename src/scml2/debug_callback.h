/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_DEBUG_CALLBACK_H
#define SCML2_DEBUG_CALLBACK_H

#include <string>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

template <typename ADAPTER, typename BASE>
class debug_callback :
  public BASE
{
public:
  debug_callback(ADAPTER adapter, const std::string& name);

  virtual unsigned int execute(tlm::tlm_generic_payload& trans);

protected:
  ADAPTER mAdapter;
};

template <typename ADAPTER, typename BASE>
inline
debug_callback<ADAPTER, BASE>::
debug_callback(ADAPTER adapter, const std::string& name) :
  BASE(name),
  mAdapter(adapter)
{
}

template <typename ADAPTER, typename BASE>
inline
unsigned int
debug_callback<ADAPTER, BASE>::
execute(tlm::tlm_generic_payload& trans)
{
  return mAdapter(trans);
}

}

#endif
