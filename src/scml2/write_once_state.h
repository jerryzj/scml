/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_WRITE_ONCE_STATE_H
#define SCML2_WRITE_ONCE_STATE_H

#include <scml2/resource.h>

namespace scml2 {

class write_once_state : public resource
{
public:
  write_once_state() : mWritable(true) {
  }

  virtual ~write_once_state() {
  }

  void set_writable(bool b) {
    mWritable = b;
  }

  bool is_writable() const {
    return mWritable;
  }

  virtual void reset() {
    set_writable(true);
  }

private:
  bool mWritable;
};

}

#endif
