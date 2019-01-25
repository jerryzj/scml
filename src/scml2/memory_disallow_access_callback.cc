/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/memory_disallow_access_callback.h>
#include <scml2/tagged_message.h>
#include <scml2/memory_base.h>
#include <tlm.h>

namespace scml2 {

void
memory_disallow_access_callback::
execute(tlm::tlm_generic_payload& trans, sc_core::sc_time&)
{
  if (mReturnError) {
    trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
  } else {
    if (trans.is_read()) {
      if (trans.get_byte_enable_ptr()) {
        for (unsigned int i = 0; i < trans.get_data_length(); ++i) {
          if (trans.get_byte_enable_ptr()[i % trans.get_byte_enable_length()]) {
            trans.get_data_ptr()[i] = 0;
          }
        }

      } else {
        memset(trans.get_data_ptr(), 0, trans.get_data_length());
      }
    }
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
  }
}

bool
memory_disallow_access_callback::
has_never_syncing_behavior() const
{
  return true;
}

}
