/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#ifndef SCML_CLOCK_CONFIGURATOR_H
#define SCML_CLOCK_CONFIGURATOR_H

#include <sysc/kernel/sc_time.h>
# include <vector>

namespace n_scml_clock {

   class engine;
   
   class configurator {
    public:
      configurator(engine* e, bool engine_enabled) : mEngine(e), mEngineEnabled(engine_enabled), mChildren() {}
      virtual ~configurator();

      virtual bool is_master() const =0;
      
      // current/active parameters
      virtual bool is_engine_enabled() const { return mEngineEnabled; }

      // by now only used during construction
      virtual sc_core::sc_time get_period() const =0;
      virtual sc_core::sc_time get_offset() const =0;
      virtual double get_duty_cycle() const =0;
      virtual bool get_posedge_first() const =0;
      virtual bool is_running() const =0;
      virtual bool is_enabled() const =0;

      virtual void set_period(const sc_core::sc_time&) =0;
      virtual void set_offset(const sc_core::sc_time&) =0;
      virtual void set_posedge_first(bool) =0;
      virtual void set_duty_cycle(double) =0;
      virtual void set_period_multiplier(double) =0;
      virtual void set_enabled(bool) =0;

      
      virtual void init() {}
      virtual void get_next_parameters(bool& running, sc_core::sc_time& period, double& duty_cycle, sc_core::sc_time& offset) =0;
      virtual sc_core::sc_time get_next_period_start_time() const =0;
      engine* get_engine() const { return mEngine; }
      
      class child {
       public:
	 virtual void handle_parent_parameters_changed(configurator*) = 0;
	 virtual void handle_parent_deleted(configurator*) = 0;
	 virtual ~child() {}
      };
      void register_child(child*);
      void unregister_child(child*);
      
    protected:
      engine* mEngine;
      bool mEngineEnabled; // the currenly active setting as the engine itself only holds the running state
      typedef std::vector<child*> tChildren;
      tChildren mChildren;
      void notify_childen();
   };

}

#endif
// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
