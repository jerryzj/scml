/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_ACCESS_RESTRICTION_B_H
#define SCML2_BITFIELD_ACCESS_RESTRICTION_B_H

#include <scml2/resource.h>

#include <string>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

class bitfield_access_restriction_b : public resource
{
public:
  bitfield_access_restriction_b(const std::string& name = "<anonymous>");
  virtual ~bitfield_access_restriction_b();

  const std::string& get_name() const;

  virtual bool has_default_restriction() const;
private:
  const std::string mName;
};

inline
bitfield_access_restriction_b::
bitfield_access_restriction_b(const std::string& name) :
  mName(name)
{
}

inline
bitfield_access_restriction_b::
~bitfield_access_restriction_b()
{
}

inline
const std::string&
bitfield_access_restriction_b::
get_name() const
{
  return mName;
}

inline 
bool
bitfield_access_restriction_b::
has_default_restriction() const
{
  return false;
}

}

#endif
