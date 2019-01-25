/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#include "simple_IPE_PV.h"
#include "simple_IPE_PV_testbench.h"

int sc_main(int argc, char**argv) {

  simple_IPE_PV I_simple_IPE_PV("I_simple_IPE_PV");
  simple_IPE_PV_testbench I_simple_IPE_PV_testbench("I_simple_IPE_PV_testbench");
 
  sc_signal<bool> s_irq_o("s_irq_o");
  sc_signal<bool> s_irq [8];

  for (unsigned int i = 0 ; i < 8 ; i++) {
    I_simple_IPE_PV.p_irq_in[i](s_irq[i]);
    I_simple_IPE_PV_testbench.p_irq_out[i](s_irq[i]); 
  }
 
  I_simple_IPE_PV.p_irq_out(s_irq_o);
  I_simple_IPE_PV_testbench.p_irq_in(s_irq_o);
 
  I_simple_IPE_PV_testbench.p_PV(I_simple_IPE_PV.p_PV);
 
  sc_start();

  return 0;
}
