/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_ROUTER_CALLBACK_BASE_H
#define SCML2_ROUTER_CALLBACK_BASE_H

#include <scml2/resource.h>

#include <string>

namespace tlm {
  class tlm_generic_payload;
}

namespace sc_core {
  class sc_time;
}

namespace scml2 {

class router_callback_base : public resource
{
public:
  router_callback_base(const std::string& name = "<anonymous>");
  virtual ~router_callback_base();

  const std::string& get_name() const;

  virtual void execute(tlm::tlm_generic_payload& trans, sc_core::sc_time& t) = 0;
  virtual bool has_never_syncing_behavior() const = 0;

private:
  const std::string mName;
};

inline
router_callback_base::
router_callback_base(const std::string& name) :
  mName(name)
{
}

inline
router_callback_base::
~router_callback_base()
{
}

inline
const std::string&
router_callback_base::
get_name() const
{
  return mName;
}

}

#endif
