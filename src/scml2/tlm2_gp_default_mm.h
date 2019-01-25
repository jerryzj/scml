/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef __TLM2_GP_DEFAULT_MM_H__
#define __TLM2_GP_DEFAULT_MM_H__

/******************************************************************************
 * Name        : tlm2_gp_default_mm
 * Description : Definition for the default version of the m_mm that goes
 *               into the osci version of the scml2 package
 * ***************************************************************************/

#include <tlm.h>
#include <scml2/tlm2_gp_mm_ifs.h>

namespace scml2 {
    
class tlm2_gp_default_mm : public tlm2_gp_mm_if
{
  public:
    virtual tlm::tlm_generic_payload* claim ()
    {
       return new tlm::tlm_generic_payload (this);
    }
    
    virtual void free (tlm::tlm_generic_payload* payload)
    {
        if (payload != NULL) {
            delete payload;
        }
    }
};
}//namespace scml2

#endif // __TLM2_GP_DEFAULT_MM_H__
