/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_LOGGING_ANDMATCH_H
#define SCML2_LOGGING_ANDMATCH_H

#include <string>

namespace scml2 { namespace logging {
  
class stream_base;

namespace match {

template <typename Match1, typename Match2>
class and_match
{
public:
  and_match(const Match1& m1, const Match2& m2);
  bool matches(const stream_base& s) const;
  std::string to_string() const;

private:
  const Match1 mMatch1;
  const Match2 mMatch2;
};

template <typename Match1, typename Match2>
inline
and_match<Match1, Match2>
operator&&(const Match1& m1, const Match2& m2)
{
  return and_match<Match1, Match2>(m1, m2);
}

template <typename Match1, typename Match2>
inline
and_match<Match1, Match2>::
and_match(const Match1& m1, const Match2& m2) :
  mMatch1(m1),
  mMatch2(m2)
{
}

template <typename Match1, typename Match2>
inline
bool
and_match<Match1, Match2>::
matches(const stream_base& s) const
{
  return mMatch1.matches(s) && mMatch2.matches(s);
}

template <typename Match1, typename Match2>
inline
std::string
and_match<Match1, Match2>::
to_string() const
{
  return "( " + mMatch1.to_string() + " && " + mMatch2.to_string() + " )";
}

} } }

#endif
