/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_IGNORE_RESTRICTION_ADAPTER_H
#define SCML2_IGNORE_RESTRICTION_ADAPTER_H

#include <scml2/types.h>

#include <cassert>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

template <typename DT>
class ignore_restriction_adapter
{
public:
  ignore_restriction_adapter(DT value);

  access_restriction_result operator()(tlm::tlm_generic_payload& trans);
  access_restriction_result operator()(DT& data, DT& bit_enables);
private:
  DT m_value;
};

template <typename DT>
inline
ignore_restriction_adapter<DT>
create_ignore_restriction_adapter(DT value = DT())
{
  return ignore_restriction_adapter<DT>(value);
}

template <typename DT>
inline
ignore_restriction_adapter<DT>::
ignore_restriction_adapter(DT value) 
  : m_value(value)
{}

template <typename DT>
inline
access_restriction_result
ignore_restriction_adapter<DT>::
operator()(tlm::tlm_generic_payload& trans)
{
  return restrict_all(trans, m_value);
}

template <typename DT>
inline
access_restriction_result
ignore_restriction_adapter<DT>::
operator()(DT& data, DT& bit_enables)
{
  return restrict_all(data, bit_enables, m_value);
}

}

#endif
