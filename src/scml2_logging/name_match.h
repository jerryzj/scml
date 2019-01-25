/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_LOGGING_NAMEMATCH_H
#define SCML2_LOGGING_NAMEMATCH_H

#include <scml2_logging/stream_base.h>

#include <string>

namespace scml2 { namespace logging {
  
class logging;

namespace match {

class name_match
{
public:
  name_match(const std::string& name);
  bool matches(const stream_base& s) const;
  std::string to_string() const;

private:
  const std::string mName;
};

inline
name_match::
name_match(const std::string& name) :
  mName(name)
{
}

inline
bool
name_match::
matches(const stream_base& s) const
{
  if (s.get_name() == mName) {
    return true;
  }
  return false;
}

inline
std::string
name_match::
to_string() const
{
  return std::string("'name' == '") + mName + "'";
}

} } }

#endif
