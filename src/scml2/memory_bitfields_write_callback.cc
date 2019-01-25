/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/memory_bitfields_write_callback.h>

#include <scml2/reg.h>

namespace scml2 {

template <typename DT>
bool
memory_bitfields_write_callback<DT>::
has_never_syncing_behavior() const
{
  return mRegister.has_never_syncing_bitfield_write_behavior();
}

template <typename DT>
inline
void
memory_bitfields_write_callback<DT>::
execute(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
  this->unroll_to_word_access(trans, t);
}

template <typename DT>
bool
memory_bitfields_write_callback<DT>::
process_unrolled_access(DT& data,
                        const DT& byteEnables,
                        sc_core::sc_time& t,
                        const tlm2_gp_extensions&)
{
  return mRegister.put_with_triggering_bitfield_callbacks(data, byteEnables, t);
}

#ifdef _WIN32
#define SCML2_INSTANTIATE_TEMPLATE(type) template class SNPS_VP_API memory_bitfields_write_callback<type>;
#else
#define SCML2_INSTANTIATE_TEMPLATE(type) template class memory_bitfields_write_callback<type>;
#endif  
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}
