/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

// Purpose: Top level wrapper module source file for timer TLM2 system

#include "systemc.h"

#include "top_TLM2.h"

//-----------------------------------------------------------------------------
// Constructor
//
top::top(sc_module_name nm) : sc_module(nm)  {
			
  // Instantiate the TLM2 modules		
  pI_Memory_TLM2 = new Memory_TLM2("pI_Memory_TLM2",
			           0x10000); // size in bytes
				
  pI_Initiator_TLM2 = new TrafficGen_TLM2("I_TrafficGen");
		
  // Bind the ports
  pI_Initiator_TLM2->socket(pI_Memory_TLM2->socket);
}


//-----------------------------------------------------------------------------
// Destructor
//
top::~top() {
  delete pI_Memory_TLM2;
  delete pI_Initiator_TLM2;
}

