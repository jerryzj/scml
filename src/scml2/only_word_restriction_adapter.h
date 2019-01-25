/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_ONLY_WORD_RESTRICTION_ADAPTER_H
#define SCML2_ONLY_WORD_RESTRICTION_ADAPTER_H

#include <scml2/types.h>

#include <cassert>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

template <typename DT>
class only_word_restriction_adapter
{
public:
  only_word_restriction_adapter(bool error, DT value);

  access_restriction_result operator()(tlm::tlm_generic_payload& trans);

 private:
  bool m_error;
  DT m_value;
};

template <typename DT>
inline
only_word_restriction_adapter<DT>
  create_only_word_restriction_adapter(bool error, DT value = DT())
{
  return only_word_restriction_adapter<DT>(error, value);
}

template <typename DT>
inline
only_word_restriction_adapter<DT>::
only_word_restriction_adapter(bool error, DT value) 
  : m_error(error)
  , m_value(value)
{}

template <typename DT>
inline
access_restriction_result
only_word_restriction_adapter<DT>::
operator()(tlm::tlm_generic_payload& trans)
{
  assert(trans.get_data_length() >= sizeOf<DT>());
  assert(trans.get_byte_enable_ptr() != 0);
  assert(trans.get_byte_enable_length() >= sizeOf<DT>());
  if ((trans.get_data_length() % sizeOf<DT>()) != 0) {
    return m_error ? RESTRICT_ERROR : restrict_all(trans, m_value);
  }
  for (unsigned i = 0; i != sizeOf<DT>(); ++i) {
    if (trans.get_byte_enable_ptr()[i] != 0xff) {
      return m_error ? RESTRICT_ERROR : restrict_all(trans, m_value);
    }
  }
  return RESTRICT_NO_ERROR;
}

}

#endif
