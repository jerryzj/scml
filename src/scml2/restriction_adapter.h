/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_RESTRICTION_ADAPTER_H
#define SCML2_RESTRICTION_ADAPTER_H

#include <scml2/types.h>

#include <cassert>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

template <typename C>
class restriction_adapter
{
public:
  typedef access_restriction_result (C::*RestrictionType)(tlm::tlm_generic_payload&);

public:
  restriction_adapter(C& c, RestrictionType cb);

  access_restriction_result operator()(tlm::tlm_generic_payload& trans);

private:
  C& mClass;
  RestrictionType mRestriction;
};

template <typename C>
inline
restriction_adapter<C>
create_restriction_adapter(C& c, typename restriction_adapter<C>::RestrictionType cb)
{
  return restriction_adapter<C>(c, cb);
}

template <typename C>
inline
restriction_adapter<C>::
restriction_adapter(C& c, RestrictionType cb) :
  mClass(c),
  mRestriction(cb)
{
}

template <typename C>
inline
access_restriction_result
restriction_adapter<C>::
operator()(tlm::tlm_generic_payload& trans)
{
  assert(mRestriction);
  return (mClass.*mRestriction)(trans);
}

}

#endif
