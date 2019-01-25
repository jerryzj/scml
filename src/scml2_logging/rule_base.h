/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_LOGGING_RULEBASE_H
#define SCML2_LOGGING_RULEBASE_H

#include <scml2_logging/snps_vp_dll.h>
#include <string>

namespace scml2 { namespace logging {

class stream_base;

class rule_base
{
public:
  virtual ~rule_base() {}
  virtual bool filter(const stream_base& s, bool previous) const = 0;
  virtual std::string to_string() const = 0;
};

} }

#endif
