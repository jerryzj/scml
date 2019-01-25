/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_DISALLOW_READ_ACCESS_CALLBACK_H
#define SCML2_BITFIELD_DISALLOW_READ_ACCESS_CALLBACK_H

#include <scml2/bitfield_read_callback_base.h>
#include <scml2/utils.h>
#include <scml2_logging/snps_vp_dll.h>

#include <tlm.h>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT>
class bitfield;

template <typename DT>
class bitfield_disallow_read_access_callback :
  public bitfield_read_callback_base<DT>
{
public:
  bitfield_disallow_read_access_callback(const bitfield<DT>& owner, bool returnError);

  virtual bool read(DT& value, const DT& bitMask, sc_core::sc_time& t);
  virtual bool has_never_syncing_behavior() const;

private:
  const bitfield<DT>& mOwner;
  bool mReturnError;
};

template <typename DT>
inline
bitfield_disallow_read_access_callback<DT>::
bitfield_disallow_read_access_callback(const bitfield<DT>& owner, bool returnError) :
  bitfield_read_callback_base<DT>(
    returnError ? "<disallow read callback>" :
                  "<ignore read callback>"),
  mOwner(owner),
  mReturnError(returnError)
{
}

#define SCML2_INSTANTIATE_TEMPLATE(type) \
   SNPS_VP_API_TMPL_CLASS(bitfield_disallow_read_access_callback<type >;)
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}

#endif
