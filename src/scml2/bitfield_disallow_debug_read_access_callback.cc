/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/bitfield_disallow_debug_read_access_callback.h>

namespace scml2 {

template <typename DT>
bool
bitfield_disallow_debug_read_access_callback<DT>::
read(DT& value, const DT&)
{
  value = 0;
  return false;
}

#ifdef _WIN32
#define SCML2_INSTANTIATE_TEMPLATE(type) \
  template class SNPS_VP_API bitfield_disallow_debug_read_access_callback<type>;
#else
#define SCML2_INSTANTIATE_TEMPLATE(type) \
  template class bitfield_disallow_debug_read_access_callback<type>;
#endif  
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}
