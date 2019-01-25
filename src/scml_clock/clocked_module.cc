/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 

#include "clocked_module.h"
#include "scml_clock.h"
#include <sysc/kernel/sc_spawn.h>
#include <iostream>
#include <algorithm>
#include <assert.h>


scml2::clocked_module::clocked_module(scml_clock_if* clock) : mEngine(0), mScheduledTick(~0ull)
{
   if (clock != 0) {
      set_clock(clock);
   }
}

scml2::clocked_module::clocked_module(sc_core::sc_in<bool>& p) : mEngine(0), mScheduledTick(~0ull)
{
   set_clock(p);
}

scml2::clocked_module::~clocked_module() {
   if (is_clock_trigger_requested()) {
      cancel_clock_trigger();
   }
}

void scml2::clocked_module::set_clock(scml_clock_if* clock) {
   mEngine = clock!=0? clock->get_tick_callback_engine() : 0;
}

void scml2::clocked_module::set_clock(sc_core::sc_in<bool>& p) {
   mEngine = scml2::get_scml_clock(p)->get_tick_callback_engine();
}

scml_clock_if* scml2::clocked_module::get_clock() const {
   return mEngine!=0? mEngine->get_master() : 0;
}


// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
