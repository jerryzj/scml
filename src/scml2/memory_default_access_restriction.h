/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_MEMORY_DEFAULT_ACCESS_RESTRICTION_H
#define SCML2_MEMORY_DEFAULT_ACCESS_RESTRICTION_H

#include <scml2/memory_access_restriction_base.h>

#include <scml2/memory_fast_access.h>
#include <scml2/toplevel_memory_base.h>

#include <tlm.h>

namespace scml2 {

template <typename DT>
class memory_default_access_restriction : public memory_access_restriction_base
{
public:
  memory_default_access_restriction();

  virtual access_restriction_result restrict(tlm::tlm_generic_payload& trans);
  virtual bool has_default_restriction() const;
};

template <typename DT>
inline
memory_default_access_restriction<DT>::
memory_default_access_restriction() 
  : memory_access_restriction_base("")
{
}

template <typename DT>
inline
access_restriction_result
memory_default_access_restriction<DT>::
restrict(tlm::tlm_generic_payload& /*trans*/)
{
  return RESTRICT_NO_ERROR;
}

template <typename DT>
inline
bool
memory_default_access_restriction<DT>::
has_default_restriction() const
{
  return true;
}

}

#endif
