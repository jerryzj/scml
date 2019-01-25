/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_READ_RESTRICTION_BASE_H
#define SCML2_BITFIELD_READ_RESTRICTION_BASE_H

#include <scml2/bitfield_access_restriction_b.h>
#include <scml2/types.h>
#include <tlm.h>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT>
class bitfield_access_restriction_base :
  public bitfield_access_restriction_b
{
public:
  typedef DT data_type;

  bitfield_access_restriction_base(const std::string& name);

  virtual access_restriction_result restrict(DT& data, DT& bitMask) = 0;
};

template <typename DT>
inline
bitfield_access_restriction_base<DT>::
bitfield_access_restriction_base(const std::string& name) :
  bitfield_access_restriction_b(name)
{
}

}

#endif
