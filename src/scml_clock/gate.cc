/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 

#include "gate.h"
#include "scml2/tagged_message_macros.h"

scml_clock_gate::scml_clock_gate(sc_core::sc_module_name n) : sc_module(n), mClock(n, clk, 1, 0) {
   SC_METHOD(handle_enable);
   sensitive << en;
   mClock.disable();
}

void scml_clock_gate::enable() {
   SCML2_ERROR(SCML_INVALID_API_USAGE) << "Error in scml_clock_gate \"" << name() << "\": scml_clock_gate needs to be disabled through the en port";
   abort();
}

void scml_clock_gate::disable() {
   SCML2_ERROR(SCML_INVALID_API_USAGE) << "Error in scml_clock_gate \"" << name() << "\": scml_clock_gate needs to be enabled through the en port";
   abort();
}

void scml_clock_gate::set_duty_cycle(double) {
   SCML2_ERROR(SCML_INVALID_API_USAGE) << "Error in scml_clock_gate \"" << name() << "\": set_duty_cycle not supporetd for scml_clock_gate" << std::endl;
   abort();
}

void scml_clock_gate::set_start_time(const sc_core::sc_time&) {
   SCML2_ERROR(SCML_INVALID_API_USAGE) << "Error in scml_clock_gate \"" << name() << "\": set_start_time not supported for scml_clock_gate" << std::endl;
   abort();
}

void scml_clock_gate::set_period(const sc_core::sc_time&) {
   SCML2_ERROR(SCML_INVALID_API_USAGE) << "Error in scml_clock_gate \"" << name() << "\": set_period not supported for scml_clock_gate" << std::endl;
   abort();
}

void scml_clock_gate::set_posedge_first(bool) {
   SCML2_ERROR(SCML_INVALID_API_USAGE) << "Error in scml_clock_gate \"" << name() << "\": set_posedge_first not supported for scml_clock_gate" << std::endl;
   abort();
}

void scml_clock_gate::set_period_multiplier(double) {
   SCML2_ERROR(SCML_INVALID_API_USAGE) << "Error in scml_clock_gate \"" << name() << "\": set_period_multiplier not supported for scml_clock_gate" << std::endl;
   abort();
}


void scml_clock_gate::handle_enable() {
   if (en.read()) {
      mClock.enable();
   } else {
      mClock.disable();
   }
}


// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
