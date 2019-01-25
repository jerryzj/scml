/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#include <scml2_logging/stream_logger.h>
#include <scml2_logging/stream_base.h>
#include <scml2_logging/severity.h>

#include <ostream>

namespace scml2 { namespace logging {

stream_logger::
stream_logger(const std::string& name, std::ostream& s) :
  logger_base(name),
  mStream(s)
{
}

void
stream_logger::
log(const stream_base& s, const char* msg)
{
  mStream << s.get_name() << "(" << s.get_severity().get_name() << "): " << msg;
}

} }
