/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_DISALLOW_ACCESS_CALLBACK_H
#define SCML2_DISALLOW_ACCESS_CALLBACK_H

#include <scml2/memory_callback_base.h>
#include <scml2_logging/snps_vp_dll.h>

namespace sc_core {
  class sc_time;
}

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

class memory_base;

class SNPS_VP_API memory_disallow_access_callback : public memory_callback_base
{
public:
  memory_disallow_access_callback(const memory_base& owner, bool returnError = true);

  virtual void execute(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
  virtual bool has_never_syncing_behavior() const;

private:
  const memory_base& mOwner;
  bool mReturnError;
};

inline
memory_disallow_access_callback::
  memory_disallow_access_callback(const memory_base& owner, bool returnError) :
  memory_callback_base(returnError ? "<disallow access callback>" :
                                     "<ignore access callback>"),
  mOwner(owner),
  mReturnError(returnError)
{
}

}

#endif
