/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_MEMORY_NULL_ACCESS_RESTRICTION_H
#define SCML2_MEMORY_NULL_ACCESS_RESTRICTION_H

#include <scml2/memory_access_restriction_base.h>

#include <scml2/memory_fast_access.h>
#include <scml2/toplevel_memory_base.h>

#include <tlm.h>

namespace scml2 {

class memory_null_access_restriction : public memory_access_restriction_base
{
public:
  memory_null_access_restriction(memory_base& memory);

  virtual access_restriction_result restrict(tlm::tlm_generic_payload& trans);
  virtual bool has_default_restriction() const;
private:
  memory_base& mMemory;
};

inline
memory_null_access_restriction::
memory_null_access_restriction(memory_base& memory) 
  : memory_access_restriction_base("")
  , mMemory(memory)
{
}

inline
access_restriction_result
memory_null_access_restriction::
restrict(tlm::tlm_generic_payload& /*trans*/)
{
  SCML2_LOG_ERROR(&mMemory, mMemory.get_streams().internal_error(), UNDEFINED_ERROR)
    << "access restriction not initialized on '" << get_name() << "'" << std::endl;
  return RESTRICT_ERROR;
}

inline
bool
memory_null_access_restriction::
has_default_restriction() const
{
  return true;
}

}

#endif
