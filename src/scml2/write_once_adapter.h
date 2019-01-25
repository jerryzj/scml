/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_WRITE_ONCE_ADAPTER_H
#define SCML2_WRITE_ONCE_ADAPTER_H

#include <scml2/write_once_state.h>
#include <scml2/reg.h>

#include <tlm.h>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT, template <typename> class MEM_OBJECT, typename ADAPTER, bool RETURN_ERROR, bool POST_WRITE>
class write_once_adapter
{
public:
  write_once_adapter(ADAPTER f, MEM_OBJECT<DT>& r) : mAdapter(f), mMemory(r) {
    mWriteOnceState = new write_once_state();
    mWriteOnceState->ref();
  }

  ~write_once_adapter() {
    mWriteOnceState->unref();
  }

  write_once_adapter(const write_once_adapter& o) : mAdapter(o.mAdapter), mMemory(o.mMemory) {
    mWriteOnceState = o.mWriteOnceState;
    mWriteOnceState->ref();
  }

  void operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time& t) {
    if (mWriteOnceState->is_writable()) {
      mWriteOnceState->set_writable(false);
      mAdapter(trans, t);
    } else {
      if (RETURN_ERROR) {
        trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
      } else {
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
      }
    }
  }

  write_once_state* get_state() const {
    return mWriteOnceState;
  }

public:
  ADAPTER mAdapter;
  MEM_OBJECT<DT>& mMemory;
  write_once_state* mWriteOnceState;
};

template <bool POST_WRITE, typename DT, template <typename> class MEM_OBJECT, typename ADAPTER>
inline
write_once_adapter<DT, MEM_OBJECT, ADAPTER, false, POST_WRITE>
create_write_once_ignore_adapter(ADAPTER f, MEM_OBJECT<DT>& r, write_once_state** state)
{
  write_once_adapter<DT, MEM_OBJECT, ADAPTER, false, POST_WRITE> result(f, r);
  *state = result.get_state();
  return result;
}

template <bool POST_WRITE, typename DT, template <typename> class MEM_OBJECT, typename ADAPTER>
inline
write_once_adapter<DT, MEM_OBJECT, ADAPTER, true, POST_WRITE>
create_write_once_error_adapter(ADAPTER f, MEM_OBJECT<DT>& r, write_once_state** state)
{
  write_once_adapter<DT, MEM_OBJECT, ADAPTER, true, POST_WRITE> result(f, r);
  *state = result.get_state();
  return result;
}

}

#endif
