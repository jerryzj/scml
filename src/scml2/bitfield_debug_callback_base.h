/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_DEBUG_CALLBACK_BASE_H
#define SCML2_BITFIELD_DEBUG_CALLBACK_BASE_H

#include <scml2/resource.h>
#include <scml2_logging/snps_vp_dll.h>
#include <string>

namespace scml2 {

class SNPS_VP_API bitfield_debug_callback_base :
  public resource
{
public:
  bitfield_debug_callback_base(const std::string& name = "<anonymous>");
  virtual ~bitfield_debug_callback_base();

  const std::string& get_name() const;

  virtual bool has_default_behavior() const;

private:
  const std::string mName;
};

inline
bitfield_debug_callback_base::
bitfield_debug_callback_base(const std::string& name) :
  mName(name)
{
}

inline
bitfield_debug_callback_base::
~bitfield_debug_callback_base()
{
}

inline
const std::string&
bitfield_debug_callback_base::
get_name() const
{
  return mName;
}

inline
bool
bitfield_debug_callback_base::
has_default_behavior() const
{
  return false;
}

}

#endif
