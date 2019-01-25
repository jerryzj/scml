/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_LOGGING_RULE_H
#define SCML2_LOGGING_RULE_H

#include <scml2_logging/rule_base.h>
#include <scml2_logging/snps_vp_dll.h>

#include <string>

namespace scml2 { namespace logging {

class stream_base;

template <typename Match>
class rule : public rule_base
{
public:
  rule(bool enableNotDisable, const Match& match);
  bool filter(const stream_base& s, bool previous) const;
  std::string to_string() const;

private:
  bool mEnableNotDisable;
  const Match mMatch;
};

template <typename Match>
inline
rule<Match>::
rule(bool enableNotDisable, const Match& match) :
  mEnableNotDisable(enableNotDisable),
  mMatch(match)
{
}

template <typename Match>
inline
bool
rule<Match>::
filter(const stream_base& s, bool previous) const
{
  if (mMatch.matches(s)) {
    return mEnableNotDisable;

  } else {
    return previous;
  }
}

template <typename Match>
inline
std::string
rule<Match>::
to_string() const
{
  if (mEnableNotDisable) {
    return std::string("enable ") + mMatch.to_string();

  } else {
    return std::string("disable ") + mMatch.to_string();
  }
}

} }

#endif
