/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_LOGGING_STREAM_BASE_S_H
#define SCML2_LOGGING_STREAM_BASE_S_H

#include <scml2_logging/stream_base.h>
#include <scml2_logging/streambuf.h>

namespace scml2 { namespace logging {

class registry;

class SNPS_VP_API stream_base_s : public stream_base
{
public:
  stream_base_s(const std::string& name, const severity& severity);
  virtual ~stream_base_s();

protected:
  streambuf mStreamBuf;

  // disabled
  stream_base_s();
  stream_base_s(const stream_base&);
  stream_base_s& operator= (const stream_base&);
};

} }

#endif
