/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_CALLBACK_OBSERVER_OWNER_H
#define SCML2_CALLBACK_OBSERVER_OWNER_H

#include <scml2/callback_observer.h>
#include <scml2/types.h>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

class callback_observer_owner {
public:
  virtual ~callback_observer_owner();
  virtual void handle_callback_changed(callback_observer* o,
                                       access_type type,
                                       access_mode mode) = 0;

  virtual void handle_begin_callback(callback_observer* o,
                                     const tlm::tlm_generic_payload& trans,
                                     access_mode mode,
                                     unsigned int id) = 0;
  virtual void handle_end_callback(callback_observer* o,
                                   const tlm::tlm_generic_payload& trans,
                                   unsigned int id) = 0;
};

inline
callback_observer_owner::
~callback_observer_owner()
{
}

}

#endif
