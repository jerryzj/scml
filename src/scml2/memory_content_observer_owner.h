/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_MEMORY_CONTENT_OBSERVER_OWNER
#define SCML2_MEMORY_CONTENT_OBSERVER_OWNER

#include <scml2/memory_content_observer.h>
#include <scml2/types.h>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

class memory_content_observer_owner {
public:
  virtual ~memory_content_observer_owner();
  virtual void handle_pre_content_access(memory_content_observer* o,
                                         access_type type,
                                         access_mode mode,
                                         unsigned long long index) = 0;
  virtual void handle_post_content_access(memory_content_observer* o,
                                          access_type type,
                                          access_mode mode,
                                          unsigned long long index) = 0;
  virtual void handle_pre_content_access(memory_content_observer* o,
                                         access_type type,
                                         access_mode mode,
                                         unsigned long long index,
										 unsigned int /* dataWidth */) {
    handle_pre_content_access(o, type, mode, index);
  }
  virtual void handle_post_content_access(memory_content_observer* o,
                                          access_type type,
                                          access_mode mode,
                                          unsigned long long index,
										  unsigned int /* dataWidth */) {
    handle_post_content_access(o, type, mode, index);
  }
};

inline
memory_content_observer_owner::
~memory_content_observer_owner()
{
}

}

#endif
