/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

// Purpose: Top level wrapper module header file for timer TLM2 system

#ifndef _TOP_TLM2_H
#define _TOP_TLM2_H

#include "systemc.h"

#include "Memory_TLM2.h"
#include "TrafficGen_TLM2.h"

//------------------------------------------------------------------------------
// class top
// top level module definition for Memory and Traffic Gen TLM2 system
//
class top : public sc_module {
public:
  
  // Constructor		
  top(sc_module_name nm);
	
  // Destructor	
  ~top();

private:
  // Instantiate aggregate members
  Memory_TLM2 * pI_Memory_TLM2;
  TrafficGen_TLM2 * pI_Initiator_TLM2;

};


#endif /* _TOP__TLM2_H */
