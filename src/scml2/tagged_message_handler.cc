/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <iostream>
#include <sstream>

#include <scml2/tagged_message.h>


namespace scml2 {
  std::ostream& info(const sc_core::sc_module* /*mod*/, eInfoTypes /*type*/, tlm::tlm_generic_payload* /*trans*/) {
    return std::cout;
  }
  
  std::ostream& warning(const sc_core::sc_module* /*mod*/, eWarningTypes /*type*/, tlm::tlm_generic_payload* /*trans*/) {
    return std::cout;
  }
  
  std::ostream& error(const sc_core::sc_module* /*mod*/, eErrorTypes /*type*/,tlm::tlm_generic_payload* /*trans*/) {
    return std::cout;
  }
  
  std::ostream& model_internal_log(const sc_core::sc_module* /*mod*/, eModelInternalTypes /*type*/,tlm::tlm_generic_payload* /*trans*/) {
    return std::cout;
  }
}
