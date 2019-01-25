/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML_BITFIELD_CALLBACK_OBSERVER_OWNER_H
#define SCML_BITFIELD_CALLBACK_OBSERVER_OWNER_H

#include "scmlinc/scml_bitfield_callback_observer.h"
#include "scmlinc/scml_types.h"

namespace sc_core {
  class sc_time;
}

class scml_bitfield_callback_observer_owner {
public:
  virtual ~scml_bitfield_callback_observer_owner();

  virtual void handle_begin_callback(scml_bitfield_callback_observer* o,
                                     const unsigned int* data,
                                     scml::AccessType type,
                                     scml::AccessMode mode,
                                     unsigned int id) = 0;
  virtual void handle_end_callback(scml_bitfield_callback_observer* o,
                                   const unsigned int* data,
                                   unsigned int id) = 0;
};

inline
scml_bitfield_callback_observer_owner::
~scml_bitfield_callback_observer_owner()
{
}

#endif
