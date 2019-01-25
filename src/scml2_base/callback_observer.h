/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_BASE_CALLBACK_OBSERVER_H_INCLUDED
#define SCML2_BASE_CALLBACK_OBSERVER_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

namespace scml2 { namespace base {

    template<typename... Args>
    class callback_observer
    {
    public:
      callback_observer() {}
      virtual ~callback_observer() {}
      virtual void callback_called(Args... args) = 0;
    };

  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_BASE_CALLBACK_OBSERVER_H_INCLUDED
