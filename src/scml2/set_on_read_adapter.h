/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_SET_ON_READ_ADAPTER_H
#define SCML2_SET_ON_READ_ADAPTER_H

#include <scml2/reg.h>

#include <tlm.h>
#include <cassert>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT, typename ADAPTER>
class set_on_read_adapter
{
public:
  set_on_read_adapter(ADAPTER f, reg<DT>& r, DT data) : mAdapter(f), mRegister(r), mData(data) {
  }

  void operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time& t) {
    assert(trans.is_read());
    mAdapter(trans, t);
    if (trans.get_response_status() == tlm::TLM_OK_RESPONSE) {
      for (unsigned int i = 0; i < trans.get_data_length(); ++i) {
        if (!trans.get_byte_enable_ptr() || trans.get_byte_enable_ptr()[i % trans.get_byte_enable_length()]) {
          mRegister.put(trans.get_address() + i % trans.get_streaming_width(), (unsigned char*)&mData, 1, 0, 0);
        }
      }
    }
  }

public:
  ADAPTER mAdapter;
  reg<DT>& mRegister;
  DT mData;
};

template <typename DT, typename ADAPTER>
inline
set_on_read_adapter<DT, ADAPTER>
create_set_on_read_adapter(ADAPTER f, reg<DT>& r, DT data)
{
  return set_on_read_adapter<DT, ADAPTER>(f, r, data);
}

}

#endif
