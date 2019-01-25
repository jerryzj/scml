/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_STATUS_OBSERVER_OWNER_H
#define SCML2_STATUS_OBSERVER_OWNER_H

#include <scml2/status_observer.h>

namespace scml2 {

class status_observer_owner {
public:
  virtual ~status_observer_owner();
  virtual void handle_status_changed(status_observer* o) = 0;
};

inline
status_observer_owner::
~status_observer_owner()
{
}

}

#endif
