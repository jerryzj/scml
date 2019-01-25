/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_LOGGING_STREAM_H
#define SCML2_LOGGING_STREAM_H

#include <scml2_logging/stream_base_s.h>

#include <ostream>
#include <string>

#define SCML2_LOG(s) \
if (!(s).is_enabled()) { \
} else \
 (s)

#define SCML2_LOG_ASSERT(a, s) \
if (!(s).is_enabled() || (a)) { \
} else \
 (s)

namespace scml2 { namespace logging {

class severity;

class stream :
  public stream_base_s,
  public std::ostream
{
public:
  stream(const std::string& name, const severity& severity);

private:
  // disabled
  stream();
  stream(const stream&);
  stream& operator= (const stream&);
};

inline
stream::
stream(const std::string& name, const severity& severity) :
  stream_base_s(name, severity),
  std::ostream(&mStreamBuf)
{
}

} }

#endif
