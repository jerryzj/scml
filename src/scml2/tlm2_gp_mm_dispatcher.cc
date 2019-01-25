/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/tlm2_gp_mm_dispatcher.h> 
#include <scml2/tlm2_gp_default_mm_creator.h>

namespace scml2 {
    
tlm2_gp_mm_dispatcher :: 
tlm2_gp_mm_dispatcher ()
    : m_mm_creator (new tlm2_gp_default_mm_creator())

// This is a singleton so tell coverity to shut up
// coverity[ctor_dtor_leak]
{
    //TODO
}

tlm2_gp_mm_dispatcher& tlm2_gp_mm_dispatcher::
get_instance() 
{
    static tlm2_gp_mm_dispatcher dispatcher;
    return dispatcher;
}
    
tlm2_gp_mm_if* tlm2_gp_mm_dispatcher::create_mm () 
{
    tlm2_gp_mm_dispatcher& dispatcher = tlm2_gp_mm_dispatcher::get_instance();
    return dispatcher.m_mm_creator->create_mm();
}


//get set functions for the creator
void tlm2_gp_mm_dispatcher::
set_tlm2_gp_mm_creator (tlm2_gp_mm_creator_if* creator)
{
    if (m_mm_creator != NULL) {
        delete m_mm_creator;
        m_mm_creator = NULL;
    }

    m_mm_creator = creator;
}


} // namespace scml2
