/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_RESTRICTION_H
#define SCML2_MEMORY_RESTRICTION_H

#include <scml2/types.h>

#include <string>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

template <typename ADAPTER, typename BASE>
class memory_restriction :
  public BASE
{
public:
  memory_restriction(ADAPTER adapter, const std::string& name);

  virtual access_restriction_result restrict(tlm::tlm_generic_payload& trans);
protected:
  ADAPTER mAdapter;
};

template <typename ADAPTER, typename BASE>
inline
memory_restriction<ADAPTER, BASE>::
memory_restriction(ADAPTER adapter, const std::string& name) :
  BASE(name),
  mAdapter(adapter)
{
}

template <typename ADAPTER, typename BASE>
inline
access_restriction_result
memory_restriction<ADAPTER, BASE>::
restrict(tlm::tlm_generic_payload& trans)
{
  return mAdapter(trans);
}

}

#endif
