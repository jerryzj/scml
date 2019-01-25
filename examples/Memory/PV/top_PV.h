/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

// Purpose: Top level wrapper module header file for timer PV system

#ifndef _TOP_PV_H
#define _TOP_PV_H

#include "systemc.h"

#include "Memory_PV.h"
#include "TrafficGen_PV.h"

//------------------------------------------------------------------------------
// class top
// top level module definition for Memory and Traffic Gen PV system
//
class top : public sc_module {
public:
  
  // Constructor		
  top(sc_module_name nm);
	
  // Destructor	
  ~top();

private:
  // Instantiate aggregate members
  Memory_PV * pI_Memory_PV;
  TrafficGen_PV * pI_TrafficGen_PV;

};


#endif /* _TOP_PV_H */
