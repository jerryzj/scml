/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_ON_WRITE_ADAPTER_H
#define SCML2_ON_WRITE_ADAPTER_H

#include <scml2/reg.h>

#include <tlm.h>
#include <cassert>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT, typename ADAPTER, class OPERATOR, bool POST_WRITE>
class on_write_adapter
{
public:
  on_write_adapter(ADAPTER f, reg<DT>& r) : mAdapter(f), mRegister(r) {
  }

  void operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time& t) {
    assert(trans.is_write());

    if (!POST_WRITE) {
      mAdapter(trans, t);
      if (trans.get_response_status() != tlm::TLM_OK_RESPONSE) {
        return;
      }
    }

    OPERATOR op;
    for (unsigned int i = 0; i < trans.get_data_length(); ++i) {
      if (!trans.get_byte_enable_ptr() || trans.get_byte_enable_ptr()[i % trans.get_byte_enable_length()]) {
        unsigned long long index = trans.get_address() + i % trans.get_streaming_width();
        unsigned char value;
        mRegister.get(index, &value, 1);
        op(value, trans.get_data_ptr()[i]);
        mRegister.put(index, &value, 1);
      }
    }

    if (POST_WRITE) {
      mAdapter(trans, t);
    }
  }

public:
  ADAPTER mAdapter;
  reg<DT>& mRegister;
};


struct set_on_write_0_functor {
  void operator()(unsigned char& value, const unsigned char& data) const {
    value |= ~data;
  }
};

struct set_on_write_1_functor {
  void operator()(unsigned char& value, const unsigned char& data) const {
    value |= data;
  }
};

struct clear_on_write_0_functor {
  void operator()(unsigned char& value, const unsigned char& data) const {
    value &= data;
  }
};

struct clear_on_write_1_functor {
  void operator()(unsigned char& value, const unsigned char& data) const {
    value &= ~data;
  }
};


template <bool POST_WRITE, typename DT, typename ADAPTER>
inline
on_write_adapter<DT, ADAPTER, clear_on_write_0_functor, POST_WRITE>
create_clear_on_write_0_adapter(ADAPTER f, reg<DT>& r)
{
  return on_write_adapter<DT, ADAPTER, clear_on_write_0_functor, POST_WRITE>(f, r);
}

template <bool POST_WRITE, typename DT, typename ADAPTER>
inline
on_write_adapter<DT, ADAPTER, clear_on_write_1_functor, POST_WRITE>
create_clear_on_write_1_adapter(ADAPTER f, reg<DT>& r)
{
  return on_write_adapter<DT, ADAPTER, clear_on_write_1_functor, POST_WRITE>(f, r);
}

template <bool POST_WRITE, typename DT, typename ADAPTER>
inline
on_write_adapter<DT, ADAPTER, set_on_write_0_functor, POST_WRITE>
create_set_on_write_0_adapter(ADAPTER f, reg<DT>& r)
{
  return on_write_adapter<DT, ADAPTER, set_on_write_0_functor, POST_WRITE>(f, r);
}

template <bool POST_WRITE, typename DT, typename ADAPTER>
inline
on_write_adapter<DT, ADAPTER, set_on_write_1_functor, POST_WRITE>
create_set_on_write_1_adapter(ADAPTER f, reg<DT>& r)
{
  return on_write_adapter<DT, ADAPTER, set_on_write_1_functor, POST_WRITE>(f, r);
}

}

#endif
