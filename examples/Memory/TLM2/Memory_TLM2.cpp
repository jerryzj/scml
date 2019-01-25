/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

// Purpose: TLM2 SCML model of a simple byte addressed memory peripheral

#include "Memory_TLM2.h"

//------------------------------------------------------------------------------
// Constructor
//
Memory_TLM2::Memory_TLM2(sc_module_name name,
				   unsigned int Size )
  : sc_module(name)
  , socket("socket")
  , m_MemoryBank("m_MemoryBank", scml_memsize(Size >> 2) ) 
  , p_adaptor("p_adaptor")
{
  
  // Bind the TLM2 target socket to the memory object
  socket(p_adaptor.pIn);
  p_adaptor.pOut(m_MemoryBank);

} //Memory_TLM2()

//------------------------------------------------------------------------------
// Destructor
//
Memory_TLM2::~Memory_TLM2() {

}

//------------------------------------------------------------------------------
// start_of_simulation()
// This is called automatically by the kernel
//
void Memory_TLM2::start_of_simulation() {

  cout << this->name() << " : start of simulation" << endl;
  
}

