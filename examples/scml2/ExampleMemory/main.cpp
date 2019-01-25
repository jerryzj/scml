/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#include <ExampleMemory.h>
#include <ExampleMemory_new.h>
#include <MemoryCheck.h>
#include <systemc>

int sc_main(int argc, char* argv[])
{
  MemoryCheck memoryCheck("memoryCheck");
  MemoryCheck memoryCheck_1("memoryCheck_1");
  ExampleMemory exampleMemory("exampleMemory");
  //Memory using new port adaptor
  ExampleMem exampleMem("exampleMem");

  memoryCheck.socket(exampleMemory.socket);
  memoryCheck_1.socket(exampleMem.socket);

  sc_core::sc_start();

  return 0;
}
