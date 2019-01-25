/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_WORD_RESTRICTION_ADAPTER_H
#define SCML2_WORD_RESTRICTION_ADAPTER_H

#include <scml2/types.h>

#include <cassert>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

template <typename M, typename C>
class word_restriction_adapter
{
public:
  typedef typename M::data_type data_type;
  typedef access_restriction_result (C::*RestrictionType)(data_type& data, data_type& bit_enables);

public:
  word_restriction_adapter(C& c, RestrictionType r, const M& mem);

  access_restriction_result operator()(tlm::tlm_generic_payload& trans);

private:
  C& mClass;
  RestrictionType mRestriction;
};

template <typename M, typename C>
inline
word_restriction_adapter<M, C>
create_word_restriction_adapter(C& c, typename word_restriction_adapter<M, C>::RestrictionType cb, const M& mem)
{
  return word_restriction_adapter<M, C>(c, cb, mem);
}

template <typename M, typename C>
inline
word_restriction_adapter<M, C>::
word_restriction_adapter(C& c, RestrictionType r, const M& /*mem*/) :
  mClass(c),
  mRestriction(r)
{
}

template <typename M, typename C>
inline
access_restriction_result
word_restriction_adapter<M, C>::
operator()(tlm::tlm_generic_payload& trans)
{
  assert(trans.get_data_length() >= sizeOf<data_type>());
  assert(trans.get_byte_enable_ptr() != 0);
  assert(trans.get_byte_enable_length() >= sizeOf<data_type>());
  data_type data = *reinterpret_cast<data_type*>(trans.get_data_ptr());
  data_type bit_enables = *reinterpret_cast<data_type*>(trans.get_byte_enable_ptr());
  assert(mRestriction);
  access_restriction_result result = (mClass.*mRestriction)(data, bit_enables);
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
