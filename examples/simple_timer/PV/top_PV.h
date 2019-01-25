/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

// Purpose: Top level for simple timer at PV level
//                    1 x simple_timer_PV
//                    1 x simple_timer_PV_TB
//                    1 x clock generator
 
#ifndef _TOP_PV_H
#define _TOP_PV_H

#include "systemc.h"

#include "simple_timer_PV.h"
#include "simple_timer_PV_TB.h"
#include "scml_clock.h"

// Start of module class definition
class top : public sc_module {
public:
  
  // Constructor	
  top(sc_module_name nm);
  
  // Destructor
  ~top();

	
private:

  simple_timer_PV * pI_simple_timer_PV;
  simple_timer_PV_TB * pI_simple_timer_PV_TB;
  
  scml_clock  * pI_scml_clock;
}; // end class top


#endif /* _TOP_PV_H */
