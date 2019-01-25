/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/memory_bitfields_debug_read_callback.h>

#include <scml2/reg.h>

#include <tlm.h>

namespace scml2 {

template <typename DT>
unsigned int
memory_bitfields_debug_read_callback<DT>::
execute(tlm::tlm_generic_payload& trans)
{
  DT data = 0;
  const DT byteEnables =
    get_bit_mask<DT>(trans.get_data_length() * 8) << trans.get_address() * 8;

  if (mRegister.get_debug_with_triggering_bitfield_callbacks(data, byteEnables)) {
    to_char_array(data >> (trans.get_address() * 8),
                  trans.get_data_ptr(),
                  trans.get_data_length(),
                  0);
    return mRegister.get_width();

  } else {
    return 0;
  }
}

#ifdef _WIN32
#define SCML2_INSTANTIATE_TEMPLATE(type)				\
  template class SNPS_VP_API memory_bitfields_debug_read_callback<type>;
#else
#define SCML2_INSTANTIATE_TEMPLATE(type)			\
  template class memory_bitfields_debug_read_callback<type>;
#endif
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}
