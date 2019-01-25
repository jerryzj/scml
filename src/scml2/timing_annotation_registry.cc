/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#include <scml2/timing_annotation_registry.h>

#include <systemc>
#include <cassert>

namespace scml2 {

timing_annotation_registry&
timing_annotation_registry::
get_instance()
{
  static timing_annotation_registry instance;
  return instance;
}

timing_annotation_registry::
timing_annotation_registry()
{
}

timing_annotation_registry::
~timing_annotation_registry()
{
}

sc_core::sc_process_b*
timing_annotation_registry::
get_current_process_handle()
{
  return sc_core::sc_get_current_process_b();
}

void
timing_annotation_registry::
register_timing_annotation(const sc_core::sc_time& t)
{
  const sc_core::sc_process_b* handle = get_current_process_handle();
  mTimingAnnotations[handle].push(&t);
}

void
timing_annotation_registry::
unregister_timing_annotation()
{
  assert(!mTimingAnnotations[get_current_process_handle()].empty());
  return mTimingAnnotations[get_current_process_handle()].pop();
}

const sc_core::sc_time&
timing_annotation_registry::
get_timing_annotation() const
{
  TimingAnnotationConstIter it =
    mTimingAnnotations.find(get_current_process_handle());
  if (it == mTimingAnnotations.end() || it->second.empty()) {
    static const sc_core::sc_time t = sc_core::SC_ZERO_TIME;
    return t;

  } else {
    assert(!it->second.empty());
    return *it->second.top();
  }
}

}
