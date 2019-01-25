/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_RESTRICTION_H
#define SCML2_BITFIELD_RESTRICTION_H

#include <scml2/types.h>

#include <string>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

template <typename ADAPTER, typename BASE>
class bitfield_restriction :
  public BASE
{
public:
  typedef typename BASE::data_type data_type;

  bitfield_restriction(ADAPTER adapter, const std::string& name);

  virtual access_restriction_result restrict(data_type& data, data_type& bitMask);
protected:
  ADAPTER mAdapter;
};

template <typename ADAPTER, typename BASE>
inline
bitfield_restriction<ADAPTER, BASE>::
bitfield_restriction(ADAPTER adapter, const std::string& name) :
  BASE(name),
  mAdapter(adapter)
{
}

template <typename ADAPTER, typename BASE>
inline
access_restriction_result
bitfield_restriction<ADAPTER, BASE>::
restrict(data_type& data, data_type& bitMask)
{
  return mAdapter(data, bitMask);
}

}

#endif
