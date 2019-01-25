/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_LOGGING_NAMEPREFIXMATCH_H
#define SCML2_LOGGING_NAMEPREFIXMATCH_H

#include <scml2_logging/stream_base.h>

#include <string>

namespace scml2 { namespace logging { namespace match {

class name_prefix_match
{
public:
  name_prefix_match(const std::string& prefix);
  bool matches(const stream_base& s) const;
  std::string to_string() const;

private:
  const std::string mPrefix;
};

inline
name_prefix_match::
name_prefix_match(const std::string& prefix) :
  mPrefix(prefix)
{
}

inline
bool
name_prefix_match::
matches(const stream_base& s) const
{
  if (mPrefix.size() > s.get_name().size()) {
    return false;
  }
  if (s.get_name().compare(0, mPrefix.size(), mPrefix) == 0) {
    return true;
  }
  return false;
}

inline
std::string
name_prefix_match::
to_string() const
{
  return std::string("'name' prefix == '") + mPrefix + "'";
}

} } }

#endif
