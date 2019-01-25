/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#ifndef SCML2_BITFIELD_DEFAULT_RESTRICTION_H
#define SCML2_BITFIELD_DEFAULT_RESTRICTION_H

#include <scml2/bitfield_access_restriction_base.h>
#include <scml2/bitfield.h>

namespace scml2 {

template <typename DT>
class bitfield_default_restriction :
  public bitfield_access_restriction_base<DT>
{
public:
  bitfield_default_restriction(bitfield<DT>& bitfield);

  virtual access_restriction_result restrict(DT& data, DT& bitMask);
  virtual bool has_default_restriction() const;
private:
  bitfield<DT>& mBitfield;
};

template <typename DT>
inline
bitfield_default_restriction<DT>::
bitfield_default_restriction(bitfield<DT>& bitfield) :
  bitfield_access_restriction_base<DT>(""),
  mBitfield(bitfield)
{
}

template <typename DT>
inline
access_restriction_result
bitfield_default_restriction<DT>::
restrict(DT& /*data*/, DT& /*bitMask*/)
{
  return RESTRICT_NO_ERROR;
}

template <typename DT>
inline
bool
bitfield_default_restriction<DT>::
has_default_restriction() const
{
  return true;
}

}

#endif
