/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 

#include <scml2_base/empty_callback_exception.h>

#ifdef SNPS_SLS_VP_BASE

namespace scml2 { namespace base {

    empty_callback_exception::empty_callback_exception(const std::string& name) 
      : std::runtime_error("The callback " + name + " was called when empty.") 
    {}
    
  }}

#endif // SNPS_SLS_VP_BASE
