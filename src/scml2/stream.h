/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_STREAM_H
#define SCML2_STREAM_H

#include <scml2/utils.h>
#include <scml2/severity.h>

#include <scml2_logging/stream.h>

namespace scml2 {

class stream : public logging::stream
{
public:
  explicit stream(const severity& severity);
  stream(const std::string& name, const severity& severity);

};

inline
stream::
stream(const severity& severity) 
  : logging::stream(create_hierarchical_sc_name(""), severity)
{
}

inline
stream::
stream(const std::string& name, const severity& severity) 
  : logging::stream(create_hierarchical_sc_name(name), severity)
{
}

}

#endif
