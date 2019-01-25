/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#ifndef SCML2_MEMORY_NULL_CALLBACK_H
#define SCML2_MEMORY_NULL_CALLBACK_H

#include <scml2/memory_callback_base.h>
#include <scml2/streams.h>

#include <scml2/tagged_message.h>
#include <scml2_logging/stream.h>

namespace tlm {
  class tlm_generic_payload;
}

namespace sc_core {
  class sc_time;
}

namespace scml2 {

class memory_null_callback : public memory_callback_base
{
public:
  memory_null_callback(memory_base& memory);
  virtual void execute(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
  virtual bool has_default_behavior() const;
  virtual bool has_never_syncing_behavior() const;

private:
  memory_base& mMemory;
};

inline
memory_null_callback::
memory_null_callback(memory_base& memory) :
  mMemory(memory)
{
}

inline
void
memory_null_callback::
execute(tlm::tlm_generic_payload&, sc_core::sc_time&)
{
  SCML2_LOG_ERROR(&mMemory, mMemory.get_streams().internal_error(), UNDEFINED_ERROR)
    << "callback not initialized on '" << get_name() << "'" << std::endl;
}

inline
bool
memory_null_callback::
has_default_behavior() const
{
  return true;
}

inline
bool
memory_null_callback::
has_never_syncing_behavior() const
{
  return true;
}

}

#endif
