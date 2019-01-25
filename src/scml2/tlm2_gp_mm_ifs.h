/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef __TLM2_GP_MM_IFS_H__
#define __TLM2_GP_MM_IFS_H__

#include <tlm.h>

/******************************************************************************
 * Name : tlm2_gp_mm_ifs
 * Description : This files defines the interfaces used by memory manager and 
 *               the memory manager creators.
 * ****************************************************************************/

namespace scml2 {

class tlm2_gp_mm_if : public tlm::tlm_mm_interface
{
  public:
    virtual tlm::tlm_generic_payload* claim() = 0 ;
};

class tlm2_gp_mm_creator_if 
{
  public:
	virtual ~tlm2_gp_mm_creator_if() {}
    virtual tlm2_gp_mm_if* create_mm () = 0; 
};

}//namespace scml2

#endif // __TLM2_GP_MM_IFS_H__
