/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_RESTRICTION_ADAPTER_TAGGED_H
#define SCML2_RESTRICTION_ADAPTER_TAGGED_H

#include <scml2/types.h>

#include <cassert>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

template <typename C>
class restriction_adapter_tagged
{
public:
  typedef access_restriction_result (C::*RestrictionType)(tlm::tlm_generic_payload&, int id);

public:
  restriction_adapter_tagged(C& c, RestrictionType cb, int id);

  access_restriction_result operator()(tlm::tlm_generic_payload& trans);

private:
  C& mClass;
  RestrictionType mRestriction;
  int mId;
};

template <typename C>
inline
restriction_adapter_tagged<C>
create_restriction_adapter_tagged(C& c, typename restriction_adapter_tagged<C>::RestrictionType cb, int id)
{
  return restriction_adapter_tagged<C>(c, cb, id);
}

template <typename C>
inline
restriction_adapter_tagged<C>::
restriction_adapter_tagged(C& c, RestrictionType cb, int id) :
  mClass(c),
  mRestriction(cb),
  mId(id)
{
}

template <typename C>
inline
access_restriction_result
restriction_adapter_tagged<C>::
operator()(tlm::tlm_generic_payload& trans)
{
  assert(mRestriction);
  return (mClass.*mRestriction)(trans, mId);
}

}

#endif
