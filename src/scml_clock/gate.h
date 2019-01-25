// -*- C++ -*-
/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML_CLOCK_GATE_H
#define SCML_CLOCK_GATE_H

#include <systemc.h>
#include "scml_clock.h"


class SNPS_VP_API scml_clock_gate : public sc_core::sc_module, public scml_clock_if, public sc_core::sc_signal_in_if<bool> {
 public:
   sc_core::sc_in<bool> clk;
   sc_core::sc_in<bool> en;
  
   SC_HAS_PROCESS(scml_clock_gate);

   scml_clock_gate(sc_core::sc_module_name);
  
   // interface sc_clock_if
   virtual const char* name() const { return sc_module::name(); }
   virtual sc_time get_period() const { return mClock.get_period(); }
   virtual sc_time get_start_time() const { return mClock.get_start_time(); }
   virtual bool get_posedge_first() const { return mClock.get_posedge_first(); }
   virtual double get_duty_cycle() const { return mClock.get_duty_cycle(); }
   virtual sc_time get_next_edge_offset(bool pos_edge) const { return mClock.get_next_edge_offset(pos_edge); }

   // interface scml_clock_if
   virtual bool running() const { return mClock.running(); }
   virtual void enable();
   virtual void disable();
   virtual bool disabled() const { return mClock.disabled(); }
   
   virtual void set_period(const  sc_core::sc_time&);
   virtual void set_posedge_first(bool posedge_first);
   virtual void set_duty_cycle(double d);
   virtual void set_start_time(const sc_core::sc_time&);
   virtual void set_period_multiplier(double);
   virtual double get_period_multiplier() const { return 1.0; }
   virtual sc_dt::uint64 get_clock_count() const { return mClock.get_clock_count(); }

   
   virtual bool check_at_tick() const { return mClock.check_at_tick(); }
   virtual unsigned long long get_tick_count() const { return mClock.get_tick_count(); }
   virtual unsigned long long get_tick_count(const sc_core::sc_time& delay) const { return mClock.get_tick_count(delay); }
   virtual sc_core::sc_time get_tick_time(long long clock_ticks_to_skip) const {
      return mClock.get_tick_time(clock_ticks_to_skip);
   }
   virtual void get_next_tick_data(const sc_core::sc_time& delay, unsigned long long& count, sc_core::sc_time& time) const {
      return mClock.get_next_tick_data(delay, count, time);
   }
   
   virtual bool register_observer(scml_clock_observer* o) { return mClock.register_observer(o); }
   virtual bool unregister_observer(scml_clock_observer* o) { return mClock.unregister_observer(o); }

   virtual bool is_master() const { return false; }
   virtual n_scml_clock::configurator* get_configurator() const { return mClock.get_configurator(); }
   virtual n_scml_clock::tick_callback_engine* get_tick_callback_engine() const { return mClock.get_tick_callback_engine(); }

   // interface sc_core::sc_signal_in_if<bool>
   virtual void register_port(sc_core::sc_port_base& b, const char* tn) { mClock.register_port(b, tn); }
   virtual const sc_core::sc_event& default_event() const { return mClock.default_event(); }
   virtual const sc_core::sc_event& value_changed_event() const { return mClock.value_changed_event(); }
   virtual const sc_core::sc_event& posedge_event() const { return mClock.posedge_event(); }
   virtual const sc_core::sc_event& negedge_event() const { return mClock.negedge_event(); }
   virtual const bool& read(void) const { return mClock.read(); }
   virtual const bool& get_data_ref() const { return mClock.get_data_ref(); }
   virtual bool event() const { return mClock.event(); }
   virtual bool posedge() const { return mClock.posedge(); }
   virtual bool negedge() const { return mClock.negedge(); }

   
 private:
   scml_clock mClock;
   void handle_enable();
};

typedef scml_clock_gate cwr_clock_gate;

#endif
// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
