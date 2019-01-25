/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_DEFAULT_BEHAVIOR_ADAPTER_H
#define SCML2_DEFAULT_BEHAVIOR_ADAPTER_H

#include <scml2/reg.h>

#include <tlm.h>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT, template <typename> class MEM_OBJECT, typename ADAPTER>
class default_behavior_adapter
{
public:
  default_behavior_adapter(ADAPTER f, MEM_OBJECT<DT>& r) : mAdapter(f), mMemory(r) {
  }

  void operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time& t) {
    mMemory.transport_without_triggering_callbacks(trans);
    mAdapter(trans, t);
  }

public:
  ADAPTER mAdapter;
  MEM_OBJECT<DT>& mMemory;
};

template <typename DT, template <typename> class MEM_OBJECT, typename ADAPTER>
inline
default_behavior_adapter<DT, MEM_OBJECT, ADAPTER>
create_default_behavior_adapter(ADAPTER f, MEM_OBJECT<DT>& r)
{
  return default_behavior_adapter<DT, MEM_OBJECT, ADAPTER>(f, r);
}

}

#endif
