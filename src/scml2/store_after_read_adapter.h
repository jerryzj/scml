/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_STORE_AFTER_READ_ADAPTER_H
#define SCML2_STORE_AFTER_READ_ADAPTER_H

#include <tlm.h>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename MEM, typename ADAPTER>
class store_after_read_adapter
{
public:
  store_after_read_adapter(ADAPTER f, MEM& m);

  void operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);

public:
  ADAPTER mAdapter;
  MEM& mMemObject;
};

template <typename MEM, typename ADAPTER>
inline
store_after_read_adapter<MEM, ADAPTER>
create_store_after_read_adapter(ADAPTER f, MEM& m)
{
  return store_after_read_adapter<MEM, ADAPTER>(f, m);
}

template <typename MEM, typename ADAPTER>
inline
store_after_read_adapter<MEM, ADAPTER>::
store_after_read_adapter(ADAPTER f, MEM& m) :
  mAdapter(f),
  mMemObject(m)
{
}

template <typename MEM, typename ADAPTER>
inline
void
store_after_read_adapter<MEM, ADAPTER>::
operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
  mAdapter(trans, t);


  if (trans.is_read() && trans.is_response_ok()) {
    if (trans.get_streaming_width() >= trans.get_data_length()) {
      mMemObject.put
	(trans.get_address(),
	 trans.get_data_ptr(),
	 trans.get_data_length(),
	 trans.get_byte_enable_ptr(),
	 trans.get_byte_enable_length());
    } else {
      for (unsigned int i = 0; i < trans.get_data_length(); ++i) {
	if (!trans.get_byte_enable_ptr() ||
	    trans.get_byte_enable_ptr()[i % trans.get_byte_enable_length()]) {
	  mMemObject.put
	    (trans.get_address() + i % trans.get_streaming_width(),
	     trans.get_data_ptr() + i,
	     1,
	     !trans.get_byte_enable_ptr() ? 0 : &trans.get_byte_enable_ptr()[i % trans.get_byte_enable_length()],
	     !trans.get_byte_enable_ptr() ? 0 : 1);
	}
      }
    }
  }
}

}

#endif
