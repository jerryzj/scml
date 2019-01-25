/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#include <scml2_logging/stream_base.h>
#include <scml2_logging/registry.h>

namespace scml2 { namespace logging {

#ifdef _WIN32
# pragma warning(push)
# pragma warning(disable: 4355)
#endif

stream_base::
stream_base(const std::string& name, const severity& severity)
: mName(name)
, mSeverity(severity)
, mEnabledOrRequiresInit(true)
, mRequiresInit(true)
{
  registry::get_instance().register_stream(this);
}

#ifdef _WIN32
# pragma warning(pop)
#endif

stream_base::
~stream_base()
{
  registry::get_instance().unregister_stream(this);
}

void
stream_base::
initialize_enabled() const
{
  if (mRequiresInit) {
    mEnabledOrRequiresInit = registry::get_instance().must_be_enabled(*this);
    mRequiresInit = false;
  }
}

} }
