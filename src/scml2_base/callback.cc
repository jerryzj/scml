/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 

#include <scml2_base/callback.h>

#ifdef SNPS_SLS_VP_BASE

namespace scml2 { namespace base {

    callback_base::callback_base(const std::string& name)
      : atom(name)
    {}

    void callback_base::call_callback_registered() {
      if (object* parent = get_parent()) {
	parent->callback_registered(*this);
      } else {
	std::cout << "WARNING: callback '" << get_name() << "' has no parent to notify that a behavior has been registered." << std::endl;
      }	
    }
    
  }}

#endif // SNPS_SLS_VP_BASE
