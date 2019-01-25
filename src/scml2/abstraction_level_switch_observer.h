/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_ABSTRACTION_LEVEL_SWITCH_OBSERVER_H
#define SCML2_ABSTRACTION_LEVEL_SWITCH_OBSERVER_H

namespace scml2 {

class abstraction_level_switch_observer
{
public:
  virtual ~abstraction_level_switch_observer() {}
  virtual void handle_abstraction_level_switched() = 0;
};

}

#endif
