/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

// Purpose: PV model of a simple byte addressed memory peripheral

#ifndef _PV_MEMORY_H
#define _PV_MEMORY_H

#include "systemc.h"

// Add in the PV interface definitions
#include "PV/PV.h"

// Include memory modeling objects
#include "scml.h"

//------------------------------------------------------------------------------
// Start of module definition:
//
class Memory_PV : public sc_module {
public :

  // Declare the PV interface port
  PVTarget_port< unsigned int, unsigned int > p_Comms;
    
  SC_HAS_PROCESS(Memory_PV);
  
  Memory_PV(sc_module_name name,
            unsigned int Size,
            unsigned int AddressingMode = 8);
    
  
  // --- Destructor
  ~Memory_PV();

private :
    
  // --- Process functions and methods
  // no processes in this simple peripheral

  // --- Kernel base methods
  void start_of_simulation();
  
  // memory bank
  scml_memory<unsigned int> m_MemoryBank;

}; // end class Memory_PV


#endif /* _PV_MEMORY_H */
