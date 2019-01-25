/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_SELECT_CALLBACK_BASE_H
#define SCML2_MEMORY_SELECT_CALLBACK_BASE_H

#include <scml2/resource.h>

#include <string>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

class mappable_if;

class memory_select_callback_base : public resource
{
public:
  memory_select_callback_base(const std::string& name = "<anonymous>");
  virtual ~memory_select_callback_base();

  const std::string& get_name() const;

  virtual mappable_if* execute(tlm::tlm_generic_payload& trans) = 0;

private:
  const std::string mName;
};

inline
memory_select_callback_base::
memory_select_callback_base(const std::string& name) :
  mName(name)
{
}

inline
memory_select_callback_base::
~memory_select_callback_base()
{
}

inline
const std::string&
memory_select_callback_base::
get_name() const
{
  return mName;
}

}

#endif
