/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML_MAPPED_RANGES_OBSERVER_OWNER_H
#define SCML_MAPPED_RANGES_OBSERVER_OWNER_H

#include "scml_mapped_ranges_observer.h"

class scml_mapped_ranges_observer_owner {
public:
  virtual ~scml_mapped_ranges_observer_owner();
  virtual void handle_mapped_ranges_changed(scml_mapped_ranges_observer* o) = 0;
};

inline
scml_mapped_ranges_observer_owner::
~scml_mapped_ranges_observer_owner()
{
}

#endif
