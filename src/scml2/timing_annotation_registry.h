/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_TIMING_ANNOTATION_REGISTRY_H
#define SCML2_TIMING_ANNOTATION_REGISTRY_H

#include <scml2_logging/snps_vp_dll.h>
#include <map>
#include <stack>

namespace sc_core {
  class sc_process_b;
  class sc_time;
}

namespace scml2 {

class SNPS_VP_API timing_annotation_registry
{
public:
  static timing_annotation_registry& get_instance();

  void register_timing_annotation(const sc_core::sc_time& t);
  void unregister_timing_annotation();

  const sc_core::sc_time& get_timing_annotation() const;

private:
  timing_annotation_registry();
  ~timing_annotation_registry();

  static sc_core::sc_process_b* get_current_process_handle();

private:
  typedef std::map<const sc_core::sc_process_b*,
                   std::stack<const sc_core::sc_time*> > TimingAnnotation;
  typedef TimingAnnotation::iterator TimingAnnotationIter;
  typedef TimingAnnotation::const_iterator TimingAnnotationConstIter;
  TimingAnnotation mTimingAnnotations;
};

}

#endif
