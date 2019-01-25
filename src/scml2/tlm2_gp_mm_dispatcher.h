/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef __TLM2_GP_MM_DISPATCHER_H__
#define __TLM2_GP_MM_DISPATCHER_H__

/******************************************************************************
 * Name        : tlm2_gp_mm_dispatcher
 * Description : This file defines a class for the memory manager object.
 *               Any component willing to instantiate a memory objects does 
 *               so using the create_mm() function of this class.
 *
 *               When used in osci the dispatcher returns the default manager when 
 *               create_mm() is called.
 *               
 *               When used in snps environment the dispatcher allows the
 *               creator to be overriden and returns the memory manager created 
 *               by the overriden creator.
 * ***************************************************************************/


#include <tlm.h>
#include <scml2/tlm2_gp_mm_ifs.h>
#include <scml2_logging/snps_vp_dll.h>

namespace scml2 {
    
class SNPS_VP_API tlm2_gp_mm_dispatcher 
{
  public: 
    static tlm2_gp_mm_dispatcher& get_instance();
    static tlm2_gp_mm_if* create_mm();
    
    //get set functions for the creator
    void set_tlm2_gp_mm_creator (tlm2_gp_mm_creator_if* creator);
 private:
   
    tlm2_gp_mm_dispatcher();
    tlm2_gp_mm_dispatcher(const tlm2_gp_mm_dispatcher&);
    tlm2_gp_mm_dispatcher& operator=(const tlm2_gp_mm_dispatcher&);

 private:
    tlm2_gp_mm_creator_if*  m_mm_creator;
};

}//namespace scml2
#endif //__TLM2_GP_MM_DISPATCHER_H__
