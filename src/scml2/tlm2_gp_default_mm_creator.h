/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef __TLM2_GP_DEFAULT_MM_CREATOR_H__
#define __TLM2_GP_DEFAULT_MM_CREATOR_H__

/******************************************************************************
 *
 * Name        : tlm2_gp_default_mm_creator
 * Description : Definition for the creator of the default memory manager for 
 *               osci version
 * ***************************************************************************/
#include <scml2/tlm2_gp_mm_ifs.h>
#include <scml2/tlm2_gp_default_mm.h>

namespace scml2 {
class tlm2_gp_default_mm_creator : public tlm2_gp_mm_creator_if
{
  public:
   virtual ~tlm2_gp_default_mm_creator() {}
    virtual tlm2_gp_mm_if* create_mm ()
    {
        return new tlm2_gp_default_mm();
    }
};
        
}//namespace scml2
#endif // __TLM2_GP_DEFAULT_MM_CREATOR_H__
