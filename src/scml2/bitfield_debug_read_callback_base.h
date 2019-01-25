/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_DEBUG_READ_CALLBACK_BASE_H
#define SCML2_BITFIELD_DEBUG_READ_CALLBACK_BASE_H

#include <scml2/bitfield_debug_callback_base.h>

namespace scml2 {

template <typename DT>
class bitfield_debug_read_callback_base :
  public bitfield_debug_callback_base
{
public:
  bitfield_debug_read_callback_base(const std::string& name);

  virtual bool read(DT& value, const DT& bitMask) = 0;
};

template <typename DT>
inline
bitfield_debug_read_callback_base<DT>::
bitfield_debug_read_callback_base(const std::string& name) :
  bitfield_debug_callback_base(name)
{
}

}

#endif
