/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#ifndef SCML2_ROUTER_NULL_CALLBACK_H
#define SCML2_ROUTER_NULL_CALLBACK_H

#include <scml2/router_callback_base.h>
#include <scml2/router_base.h>
#include <scml2/streams.h>

#include <scml2/tagged_message.h>
#include <scml2_logging/stream.h>
#include <tlm.h>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

class router_null_callback : public router_callback_base
{
public:
  router_null_callback(router_base& router);

  virtual void execute(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
  virtual bool has_never_syncing_behavior() const;

private:
  router_base& mRouter;
};

inline
router_null_callback::
router_null_callback(router_base& router) :
  mRouter(router)
{
}

inline
void
router_null_callback::
execute(tlm::tlm_generic_payload& trans, sc_core::sc_time&)
{
  SCML2_LOG_ERROR(&mRouter, mRouter.get_streams().error(), SCML_INVALID_API_USAGE)
    << "No callback registered: Unmapped address = 0x"
    << std::hex << trans.get_address() << std::dec
    << std::endl;
}

inline
bool
router_null_callback::
has_never_syncing_behavior() const
{
  return true;
}

}

#endif
