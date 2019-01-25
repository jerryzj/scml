/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_TLM2_GP_EXTENSIONS_H
#define SCML2_TLM2_GP_EXTENSIONS_H

#include <tlm.h>

namespace scml2 {

class tlm2_gp_extensions {
public:
  tlm2_gp_extensions(const tlm::tlm_generic_payload& payload) : payload(payload) {
  }

  template<typename T> T* get_extension() const {
    return payload.get_extension<T>();
  }

private:
  const tlm::tlm_generic_payload& payload;
};

}

#endif
