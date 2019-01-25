/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_WRITE_ONCE_RESTRICTION_ADAPTER_H
#define SCML2_WRITE_ONCE_RESTRICTION_ADAPTER_H

#include <scml2/types.h>
#include <scml2/write_once_state.h>

#include <cassert>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

template <typename DT>
class write_once_restriction_adapter
{
public:
  write_once_restriction_adapter(bool error, DT value, write_once_state** state);
  write_once_restriction_adapter(const write_once_restriction_adapter& other);
  ~write_once_restriction_adapter();

  access_restriction_result operator()(tlm::tlm_generic_payload& trans);
  access_restriction_result operator()(DT& data, DT& bit_enables);

 private:
  write_once_state* m_state;
  bool m_already_written;
  const bool m_error;
  const DT m_value;
};

template <typename DT>
inline
write_once_restriction_adapter<DT>
create_write_once_restriction_adapter(bool error, DT value, write_once_state** state)
{
  return write_once_restriction_adapter<DT>(error, value, state);
}

template <typename DT>
inline
write_once_restriction_adapter<DT>::
write_once_restriction_adapter(bool error, DT value, write_once_state** state) 
  : m_state(new write_once_state())
  , m_error(error)
  , m_value(value)
{
  m_state->ref();
  *state = m_state;
}

template <typename DT>
inline
write_once_restriction_adapter<DT>::
write_once_restriction_adapter(const write_once_restriction_adapter& other)
  : m_state(other.m_state)
  , m_error(other.m_error)
  , m_value(other.m_value)
{
  m_state->ref();
}
  
template <typename DT>
inline
write_once_restriction_adapter<DT>::
~write_once_restriction_adapter()
{
  m_state->unref();
}

template <typename DT>
inline
access_restriction_result
write_once_restriction_adapter<DT>::
operator()(tlm::tlm_generic_payload& trans)
{
  if (!m_state->is_writable()) {
    return m_error ? RESTRICT_ERROR : restrict_all(trans, m_value);
  } else {
    m_state->set_writable(false);
    return RESTRICT_NO_ERROR;
  }
}

template <typename DT>
inline
access_restriction_result
write_once_restriction_adapter<DT>::
operator()(DT& data, DT& bit_enables)
{
  if (!m_state->is_writable()) {
    return m_error ? RESTRICT_ERROR : restrict_all(data, bit_enables, m_value);
  } else {
    m_state->set_writable(false);
    return RESTRICT_NO_ERROR;
  }
}

}

#endif
