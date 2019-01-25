/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 

#ifndef EXAMPLE_MEM_NEW_H
#define EXAMPLE_MEM_NEW_H

#include <scml2.h>
#include <tlm.h>
#include <systemc>

class ExampleMem : public sc_core::sc_module
{
private:
  static const unsigned int MEMORY_SIZE = 0x10000;

public:
  tlm::tlm_target_socket<32> socket;

public:
  SC_HAS_PROCESS(ExampleMem);

  explicit ExampleMem(sc_core::sc_module_name name) :
    sc_core::sc_module(name),
    socket("socket"),
    //mAdapter("mAdapter", socket),
    mMemory("mMemory", MEMORY_SIZE >> 2)
  {
    p_adaptor = scml2::target_port_adaptor::create("p_adaptor", &socket);
  (*p_adaptor)(mMemory);
    //mAdapter(mMemory);
  }

private:
//  scml2::tlm2_gp_target_adapter<32> mAdapter;
 scml2::target_port_adaptor* p_adaptor;
  scml2::memory<unsigned int> mMemory;
};

#endif
