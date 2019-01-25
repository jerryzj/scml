/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_OBJECT_BASE_NAMED_BASE_H_INCLUDED
#define SCML2_OBJECT_BASE_NAMED_BASE_H_INCLUDED

#include "scml2_base/utils.h"

#ifdef SNPS_SLS_VP_BASE

#include "scml2_base/named_base.h"

namespace scml2 { namespace base {
    
    class object;

    class SNPS_VP_API atom 
      : public named_base
    {
    public:
      virtual ~atom();
      atom();
      atom(const std::string& name);
      object* get_parent() const;
    private:
      object* m_parent;
    };

  }}

#endif // SNPS_SLS_VP_BASE

#endif // SCML2_OBJECT_BASE_NAMED_BASE_H_INCLUDED
