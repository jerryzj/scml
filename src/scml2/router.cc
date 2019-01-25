/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#include <scml2/router.h>

namespace scml2 {

template <typename DT>
router<DT>::
router(const std::string& name,
       unsigned long long size) :
  router_base(name, size, sizeOf<DT>())
{
}

template <typename DT>
router<DT>::
~router()
{
}

#ifdef _WIN32
#define SCML2_INSTANTIATE_TEMPLATE(type) template class SNPS_VP_API router<type >;
#else
#define SCML2_INSTANTIATE_TEMPLATE(type) template class router<type >;
#endif  
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}
