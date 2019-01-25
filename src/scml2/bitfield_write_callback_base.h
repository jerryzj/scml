/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_WRITE_CALLBACK_BASE_H
#define SCML2_BITFIELD_WRITE_CALLBACK_BASE_H

#include <scml2/bitfield_callback_base.h>
#include <tlm.h>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT>
class bitfield_write_callback_base :
  public bitfield_callback_base
{
public:
  bitfield_write_callback_base(const std::string& name);

  virtual bool write(const DT& value, const DT& bitMask, sc_core::sc_time& t) = 0;
};

template <typename DT>
inline
bitfield_write_callback_base<DT>::
bitfield_write_callback_base(const std::string& name) :
  bitfield_callback_base(name)
{
}

}

#endif
