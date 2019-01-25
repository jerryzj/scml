/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_ERROR_RESTRICTION_ADAPTER_H
#define SCML2_ERROR_RESTRICTION_ADAPTER_H

#include <scml2/types.h>

#include <cassert>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

template <typename DT>
class error_restriction_adapter
{
public:
  error_restriction_adapter();

  access_restriction_result operator()(tlm::tlm_generic_payload& trans);
  access_restriction_result operator()(DT& data, DT& bit_enables);
};

template <typename DT>
inline
error_restriction_adapter<DT>
create_error_restriction_adapter()
{
  return error_restriction_adapter<DT>();
}

template <typename DT>
inline
error_restriction_adapter<DT>::
error_restriction_adapter() 
{}

template <typename DT>
inline
access_restriction_result
error_restriction_adapter<DT>::
operator()(tlm::tlm_generic_payload& /*trans*/)
{
  return RESTRICT_ERROR;
}

template <typename DT>
inline
access_restriction_result
error_restriction_adapter<DT>::
operator()(DT& /*data*/, DT& /*bit_enables*/)
{
  return RESTRICT_ERROR;
}

}

#endif
