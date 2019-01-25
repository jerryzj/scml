/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_WORD_RESTRICTION_ADAPTER_TAGGED_H
#define SCML2_WORD_RESTRICTION_ADAPTER_TAGGED_H

#include <scml2/types.h>

#include <cassert>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

template <typename M, typename C>
class word_restriction_adapter_tagged
{
public:
  typedef typename M::data_type data_type;
  typedef access_restriction_result (C::*RestrictionType)(data_type& data, data_type& bit_enables, int id);

public:
  word_restriction_adapter_tagged(C& c, RestrictionType r, int id, const M& mem);

  access_restriction_result operator()(tlm::tlm_generic_payload& trans);

private:
  C& mClass;
  RestrictionType mRestriction;
  int mId;
};

template <typename M, typename C>
inline
word_restriction_adapter_tagged<M, C>
create_word_restriction_adapter_tagged(C& c, typename word_restriction_adapter_tagged<M, C>::RestrictionType cb, int id, const M& mem)
{
  return word_restriction_adapter_tagged<M, C>(c, cb, id, mem);
}

template <typename M, typename C>
inline
word_restriction_adapter_tagged<M, C>::
word_restriction_adapter_tagged(C& c, RestrictionType r, int id, const M& /*mem*/) :
  mClass(c),
  mRestriction(r),
  mId(id)
{
}

template <typename M, typename C>
inline
access_restriction_result
word_restriction_adapter_tagged<M, C>::
operator()(tlm::tlm_generic_payload& trans)
{
  assert(trans.get_data_length() >= sizeOf<data_type>());
  assert(trans.get_byte_enable_ptr() != 0);
  assert(trans.get_byte_enable_length() >= sizeOf<data_type>());
  data_type data = *reinterpret_cast<data_type*>(trans.get_data_ptr());
  data_type bit_enables = *reinterpret_cast<data_type*>(trans.get_byte_enable_ptr());
  assert(mRestriction);
  access_restriction_result result = (mClass.*mRestriction)(data, bit_enables, mId);
  if (result == RESTRICT_NO_ERROR) {
    data_type& orig_data = *reinterpret_cast<data_type*>(trans.get_data_ptr());
    data_type& orig_bit_enables = *reinterpret_cast<data_type*>(trans.get_byte_enable_ptr());
    data_type changed_bit_enables = orig_bit_enables & (~bit_enables);
    orig_bit_enables &= bit_enables;
    orig_data = (orig_data & ~changed_bit_enables) | (data & changed_bit_enables);
  }
  return result;
}

}

#endif
