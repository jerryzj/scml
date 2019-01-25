/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_RESTRICTION_ADAPTER_H
#define SCML2_BITFIELD_RESTRICTION_ADAPTER_H

#include <scml2/types.h>

#include <cassert>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

template <typename DT, typename C>
class bitfield_restriction_adapter
{
public:
  typedef access_restriction_result (C::*RestrictionType)(DT&, DT&);

public:
  bitfield_restriction_adapter(C& c, RestrictionType cb);

  access_restriction_result operator()(DT& data, DT& bitMask);

private:
  C& mClass;
  RestrictionType mRestriction;
};

template <typename DT, typename C>
inline
bitfield_restriction_adapter<DT, C>
create_bitfield_restriction_adapter(C& c, typename bitfield_restriction_adapter<DT, C>::RestrictionType cb)
{
  return bitfield_restriction_adapter<DT, C>(c, cb);
}

template <typename DT, typename C>
inline
bitfield_restriction_adapter<DT, C>::
bitfield_restriction_adapter(C& c, RestrictionType cb) :
  mClass(c),
  mRestriction(cb)
{
}

template <typename DT, typename C>
inline
access_restriction_result
bitfield_restriction_adapter<DT, C>::
operator()(DT& data, DT& bitMask)
{
  assert(mRestriction);
  return (mClass.*mRestriction)(data, bitMask);
}

}

#endif
