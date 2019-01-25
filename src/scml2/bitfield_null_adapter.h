/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_NULL_ADAPTER_H
#define SCML2_BITFIELD_NULL_ADAPTER_H

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template<typename DT>
class bitfield_null_adapter
{
public:
  bitfield_null_adapter() {
  }

  bool operator()(const DT&, const DT&, sc_core::sc_time&) {
    return true;
  }
};

}

#endif
