/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#ifndef SCML2_PIN_CALLBACK_BASE_H
#define SCML2_PIN_CALLBACK_BASE_H

#include <systemc>
#include <sysc/kernel/sc_dynamic_processes.h>

namespace scml2 {
  template<typename T>
  class pin_callback_base {
    public:
      pin_callback_base(sc_core::sc_in<T>& /*p*/, sc_core::sc_event_finder* event) {
        sc_core::sc_spawn_options opts;
        opts.dont_initialize();
        opts.spawn_method();
        opts.set_sensitivity(event);
        sc_core::sc_spawn(sc_bind(&pin_callback_base::execute, this), sc_core::sc_gen_unique_name("execute"), &opts);
      }

      virtual void execute() = 0;
  };
}

#endif
