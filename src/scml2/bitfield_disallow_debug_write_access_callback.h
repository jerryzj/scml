/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_DISALLOW_DEBUG_WRITE_ACCESS_CALLBACK_H
#define SCML2_BITFIELD_DISALLOW_DEBUG_WRITE_ACCESS_CALLBACK_H

#include <scml2/bitfield_debug_write_callback_base.h>
#include <scml2/utils.h>
#include <scml2_logging/snps_vp_dll.h>

namespace scml2 {

template <typename DT>
class bitfield_disallow_debug_write_access_callback :
  public bitfield_debug_write_callback_base<DT>
{
public:
  bitfield_disallow_debug_write_access_callback();

  virtual bool write(const DT& value, const DT& bitMask);
};

template <typename DT>
inline
bitfield_disallow_debug_write_access_callback<DT>::
bitfield_disallow_debug_write_access_callback() :
  bitfield_debug_write_callback_base<DT>("<disallow debug write callback>")
{
}

#define SCML2_INSTANTIATE_TEMPLATE(type) \
  SNPS_VP_API_TMPL_CLASS(bitfield_disallow_debug_write_access_callback<type >;)
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}

#endif
