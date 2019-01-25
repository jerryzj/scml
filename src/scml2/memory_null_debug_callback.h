/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#ifndef SCML2_MEMORY_NULL_DEBUG_CALLBACK_H
#define SCML2_MEMORY_NULL_DEBUG_CALLBACK_H

#include <scml2/memory_debug_callback_base.h>
#include <scml2/streams.h>

#include <scml2/tagged_message.h>
#include <scml2_logging/stream.h>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

class memory_null_debug_callback : public memory_debug_callback_base
{
public:
  memory_null_debug_callback(memory_base& memory);
  virtual unsigned int execute(tlm::tlm_generic_payload& trans);
  virtual bool has_default_behavior() const;

private:
  memory_base& mMemory;
};

inline
memory_null_debug_callback::
memory_null_debug_callback(memory_base& memory) :
  mMemory(memory)
{
}

inline
unsigned int
memory_null_debug_callback::
execute(tlm::tlm_generic_payload&)
{
  SCML2_LOG_ERROR(&mMemory, mMemory.get_streams().internal_error(), UNDEFINED_ERROR)
    << "debug callback not initialized on '" << get_name() << "'" << std::endl;
  return 0;
}

inline
bool
memory_null_debug_callback::
has_default_behavior() const
{
  return true;
}

}

#endif
