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
#ifndef SCML_CLOCK_IF_H
#define SCML_CLOCK_IF_H

#include <sysc/communication/sc_clock.h>
#include <scml2_logging/snps_vp_dll.h>

namespace n_scml_clock {
   class configurator;
   class tick_callback_engine;
}

class scml_clock_observer;

class scml_clock_if : 
   public virtual sc_core::sc_interface
{
 public:
   virtual const char* name() const =0;
   virtual bool is_master() const =0;
//   virtual void reset(int n) = 0; // suspends the clock for n cycles
   virtual bool running() const =0;
   virtual void enable() =0;
   virtual void disable() =0;
   virtual bool disabled() const =0;
   
   virtual void set_period(const  sc_core::sc_time &t) =0;
   virtual void set_posedge_first(bool posedge_first) =0;
   virtual void set_duty_cycle(double d) =0;
   virtual void set_start_time(const sc_core::sc_time&) =0;
   virtual void set_period_multiplier(double) =0;
   virtual double get_period_multiplier() const =0;
   virtual sc_dt::uint64 get_clock_count() const =0;


   virtual sc_core::sc_time get_period() const =0;
   virtual sc_core::sc_time get_start_time() const =0;
   virtual bool get_posedge_first() const =0;
   virtual double get_duty_cycle() const =0;
   virtual sc_core::sc_time get_next_edge_offset(bool pos_edge) const =0;

   /** Returns true if the clock has or will tick (start of new period) at
    *  the current SystemC cycle of the current time. */
   virtual bool check_at_tick() const =0;
   /** Get the current tick count of the clock. The tick count corresponds
    *  to the number of potential clock ticks (start of new period), which
    *  is the number of ticks in a hypothetical platform in which there was
    *  a tick trigger request for each individual clock tick. A
    *  synchronized call (called at the tick time of the clock with an
    *  active tick trigger request) to this method is very fast. An
    *  unsynchronized call is more expensive, but only the first time for a
    *  given SystemC time, due to internal caching of the result .*/
   virtual unsigned long long get_tick_count() const =0;
   /** Get the tick count of the clock for the future time
    *  sc_time_stamp()+delay. For example if called with
    *  delay=SC_ZERO_TIME, it returns the current tick count, which is
    *  equal to get_tick_count(). */
   virtual unsigned long long get_tick_count(const sc_core::sc_time& delay) const =0;
   /** Get the SystemC time for the clock tick (start of next period) that
    *  happens after CLOCK_TICKS_TO_SKIP ticks from now. For example if
    *  called with CLOCK_TICKS_TO_SKIP=0, it returns the time of the next
    *  tick. */
   virtual sc_core::sc_time get_tick_time(long long clock_ticks_to_skip) const =0;
   /** Get the data for the next clock tick (start of next period) after a
    *  given future SystemC time. The future point in time is given by the
    *  sc_time argument DELAY, which is interpreted relative to the current
    *  SystemC time sc_time_stamp(). If called synchronized (for the future
    *  time of a clock tick), it returns the data for the next following
    *  clock tick. */
   virtual void get_next_tick_data(const sc_core::sc_time& delay, unsigned long long& count, sc_core::sc_time& time) const =0;
   
   virtual bool register_observer(scml_clock_observer*) =0;
   virtual bool unregister_observer(scml_clock_observer*) =0;
   
   virtual n_scml_clock::configurator* get_configurator() const =0;
   virtual n_scml_clock::tick_callback_engine* get_tick_callback_engine() const =0;

   virtual ~scml_clock_if() {}
};

class scml_clock_observer {
 public:
   virtual void handle_clock_parameters_updated(scml_clock_if*) =0;
   virtual void handle_clock_deleted(scml_clock_if*) =0;
   virtual ~scml_clock_observer() {}
};

class SNPS_VP_API scml_clock_parameter_change_control_if {
 public:
   scml_clock_parameter_change_control_if() {}
   virtual ~scml_clock_parameter_change_control_if() {}
   
   class client {
    public:
      virtual void handle_clock_parameter_change_request(scml_clock_parameter_change_control_if*) =0;
   };
   
   virtual void register_client(client*) =0;
   virtual void signal_ready_for_parameter_change(client*) =0;
   
 private:
   // Disable copy constructors
   scml_clock_parameter_change_control_if(const scml_clock_parameter_change_control_if&);
   scml_clock_parameter_change_control_if& operator= (const scml_clock_parameter_change_control_if&);
};

typedef scml_clock_if cwr_clock_if;

namespace scml2 {
   SNPS_VP_API scml_clock_if*   get_scml_clock(sc_core::sc_in<bool>& clk, bool allow_stubbed=false);
   SNPS_VP_API sc_core::sc_time get_period(sc_core::sc_in<bool>& clk);
   inline SNPS_VP_API sc_core::sc_time convert_frequency_to_period(double f) { return sc_core::sc_time(1.0/f, sc_core::SC_SEC); }


   SNPS_VP_API void bind_scml_clock_parameter_change_control(scml_clock_if* clk, scml_clock_parameter_change_control_if* d);
   SNPS_VP_API void unbind_scml_clock_parameter_change_control(scml_clock_if* clk);
   SNPS_VP_API scml_clock_parameter_change_control_if* get_scml_clock_parameter_change_control(scml_clock_if* clk);
   SNPS_VP_API scml_clock_parameter_change_control_if* get_scml_clock_parameter_change_control(sc_core::sc_in<bool>& clk_in_port);
}

#endif
// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
