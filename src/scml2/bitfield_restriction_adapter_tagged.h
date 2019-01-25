/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_RESTRICTION_ADAPTER_TAGGED_H
#define SCML2_BITFIELD_RESTRICTION_ADAPTER_TAGGED_H

#include <scml2/types.h>

#include <cassert>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

template <typename DT, typename C>
class bitfield_restriction_adapter_tagged
{
public:
  typedef access_restriction_result (C::*RestrictionType)(DT&, DT&, int);

public:
  bitfield_restriction_adapter_tagged(C& c, RestrictionType cb, int id);

  access_restriction_result operator()(DT& data, DT& bitMask);

private:
  C& mClass;
  RestrictionType mRestriction;
  int mId;
};

template <typename DT, typename C>
inline
bitfield_restriction_adapter_tagged<DT, C>
create_bitfield_restriction_adapter_tagged(C& c, typename bitfield_restriction_adapter_tagged<DT, C>::RestrictionType cb, int id)
{
  return bitfield_restriction_adapter_tagged<DT, C>(c, cb, id);
}

template <typename DT, typename C>
inline
bitfield_restriction_adapter_tagged<DT, C>::
bitfield_restriction_adapter_tagged(C& c, RestrictionType cb, int id) :
  mClass(c),
  mRestriction(cb),
  mId(id)
{
}

template <typename DT, typename C>
inline
access_restriction_result
bitfield_restriction_adapter_tagged<DT, C>::
operator()(DT& data, DT& bitMask)
{
  assert(mRestriction);
  return (mClass.*mRestriction)(data, bitMask, mId);
}

}

#endif
