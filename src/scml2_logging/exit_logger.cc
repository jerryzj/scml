/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#include <scml2_logging/exit_logger.h>
#include <scml2_logging/registry.h>

namespace scml2 { namespace logging {

exit_logger::
exit_logger() :
  logger_base("exit_logger")
{
}

void
exit_logger::
log(const stream_base&, const char*)
{
  registry::get_instance().set_exit_after_log();
}

} }
