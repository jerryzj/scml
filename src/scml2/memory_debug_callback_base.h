/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_DEBUG_CALLBACK_BASE_H
#define SCML2_MEMORY_DEBUG_CALLBACK_BASE_H

#include <scml2/resource.h>

#include <string>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

class memory_debug_callback_base : public resource
{
public:
  memory_debug_callback_base(const std::string& name = "<anonymous>");
  virtual ~memory_debug_callback_base();

  const std::string& get_name() const;

  virtual unsigned int execute(tlm::tlm_generic_payload& trans) = 0;
  virtual bool has_default_behavior() const;

private:
  const std::string mName;
};

inline
memory_debug_callback_base::
memory_debug_callback_base(const std::string& name) :
  mName(name)
{
}

inline
memory_debug_callback_base::
~memory_debug_callback_base()
{
}

inline
const std::string&
memory_debug_callback_base::
get_name() const
{
  return mName;
}

inline
bool
memory_debug_callback_base::
has_default_behavior() const
{
  return false;
}

}

#endif
