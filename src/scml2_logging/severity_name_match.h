/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_LOGGING_LEVELMATCH_H
#define SCML2_LOGGING_LEVELMATCH_H

#include <scml2_logging/stream_base.h>

#include <string>

namespace scml2 { namespace logging { namespace match {

class severity_name_match
{
public:
  severity_name_match(const std::string& severityName);
  bool matches(const stream_base& s) const;
  std::string to_string() const;

private:
  const std::string mSeverityName;
};

inline
severity_name_match::
severity_name_match(const std::string& severityName) :
  mSeverityName(severityName)
{
}

inline
bool
severity_name_match::
matches(const stream_base& s) const
{
  if (s.get_severity().get_name() == mSeverityName) {
    return true;
  }
  return false;
}

inline
std::string
severity_name_match::
to_string() const
{
  return std::string("'severity' == '") + mSeverityName + "'";
}

} } }

#endif
