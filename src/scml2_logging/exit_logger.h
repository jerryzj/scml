/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_LOGGING_EXITLOGGER_H
#define SCML2_LOGGING_EXITLOGGER_H

#include <scml2_logging/logger_base.h>
#include <scml2_logging/snps_vp_dll.h>

namespace scml2 { namespace logging {

class stream_base;

class SNPS_VP_API exit_logger : public logger_base
{
public:
  exit_logger();
  void log(const stream_base& s, const char* msg);
};

} }

#endif
