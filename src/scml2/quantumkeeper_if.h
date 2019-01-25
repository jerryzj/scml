/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_QUANTUMKEEPER_IF_H
#define SCML2_QUANTUMKEEPER_IF_H

#include <systemc>

namespace scml2 {

class quantumkeeper_if
{
public:
  virtual ~quantumkeeper_if();

  virtual void inc(const sc_core::sc_time& t) = 0;
  virtual void set(const sc_core::sc_time& t) = 0;
  virtual sc_core::sc_time get_local_time() const = 0;
};

inline
quantumkeeper_if::
~quantumkeeper_if()
{
}

}

#endif
