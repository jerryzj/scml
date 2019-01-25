/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_STREAMS_H
#define SCML2_STREAMS_H

#include <scml2_logging/stream.h>
#include <scml2_logging/severity.h>
#include <string>

namespace scml2 {

class streams
{
public:
  explicit streams(const std::string& name);
  virtual ~streams();

  logging::stream& internal_error() const;
  logging::stream& error() const;
  logging::stream& warning() const;

private:
  mutable logging::stream mInternalError;
  mutable logging::stream mError;
  mutable logging::stream mWarning;
};

inline
streams::
streams(const std::string& name) :
  mInternalError(name, logging::severity::internal_error()),
  mError(name, logging::severity::error()),
  mWarning(name, logging::severity::warning())
{
}

inline
streams::
~streams()
{
}

inline
logging::stream&
streams::
internal_error() const
{
  return mInternalError;
}

inline
logging::stream&
streams::
error() const
{
  return mError;
}

inline
logging::stream&
streams::
warning() const
{
  return mWarning;
}

}

#endif
