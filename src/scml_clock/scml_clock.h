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
#ifndef SCML_CLOCK_H
#define SCML_CLOCK_H

#include "engine.h"
#include "scml_clock_if.h"
# include <vector>
#include <scml2_logging/snps_vp_dll.h>

namespace n_scml_clock {
   class configurator;
   class on_off_control;
   class tick_callback_engine;
}

class SNPS_VP_API scml_clock : public n_scml_clock::engine, public scml_clock_if
{
 public:
   explicit scml_clock(const char* name);
   scml_clock();
   scml_clock(const char* /*name*/, const sc_core::sc_time& period, double duty_cycle=0.5,
	      const sc_core::sc_time& start_time=sc_core::SC_ZERO_TIME, bool posedge_first=true);
   scml_clock(const char* /*name*/, double period_v, sc_core::sc_time_unit period_tu, double duty_cycle,
	      double start_time_v, sc_core::sc_time_unit start_time_tu, bool posedge_first=true);
   scml_clock(const char* /*name*/, double period_v, sc_core::sc_time_unit period_tu, double duty_cycle=0.5);

   scml_clock(const char* /*name*/, scml_clock_if& clk, double period_multiplier, double start_multiplier);
   scml_clock(const char* /*name*/, sc_core::sc_in<bool>& clk, double period_multiplier, double start_multiplier, bool allow_stubbed=false);

   virtual ~scml_clock();
   
   // interface sc_clock_if
   virtual sc_core::sc_time get_period() const;
   virtual sc_core::sc_time get_start_time() const;
   virtual bool get_posedge_first() const;
   virtual double get_duty_cycle() const;
   virtual sc_core::sc_time get_next_edge_offset(bool pos_edge) const;

   // interface scml_clock_if
   virtual const char* name() const { return engine::name(); }
   virtual bool is_master() const;

   virtual bool running() const;
   virtual void enable();
   virtual void disable();
   virtual bool disabled() const;

   virtual void set_period(const sc_core::sc_time&);
   virtual void set_posedge_first(bool posedge_first);
   virtual void set_duty_cycle(double);
   virtual void set_start_time(const sc_core::sc_time&);
   virtual void set_period_multiplier(double);
   
   virtual double get_period_multiplier() const;
   
   virtual sc_dt::uint64 get_clock_count() const;


   virtual bool check_at_tick() const;
   virtual unsigned long long get_tick_count() const;
   virtual unsigned long long get_tick_count(const sc_core::sc_time& delay) const;
   virtual sc_core::sc_time get_tick_time(long long clock_ticks_to_skip) const;
   virtual void get_next_tick_data(const sc_core::sc_time& delay, unsigned long long& count, sc_core::sc_time& time) const;
   
   virtual n_scml_clock::configurator* get_configurator() const { return engine::get_configurator(); }
   virtual n_scml_clock::tick_callback_engine* get_tick_callback_engine() const;
   
   virtual const char* kind() const { return "scml_clock"; }
   virtual void dump(std::ostream&) const;

   virtual bool register_observer(scml_clock_observer*);
   virtual bool unregister_observer(scml_clock_observer*);
   
   // for compatibility with sc_clock
   sc_core::sc_time period()        const { return get_period(); }
   double           duty_cycle()    const { return get_duty_cycle(); }
   bool             posedge_first() const { return get_posedge_first(); }
   sc_core::sc_time start_time()    const { return get_start_time(); }

   virtual void register_port(sc_core::sc_port_base& b, const char* tn) { engine::register_port(b, tn); }
   virtual const sc_core::sc_event& default_event() const { return engine::default_event(); }
   
   // for backward compatibility with 1.0, times in default time units
   scml_clock(const char* name, double period, double duty_cycle=0.5, double start_time=0.0, bool posedge_first=true);
   
 protected:
   scml_clock(const char* name, scml_clock_if* clk, double period_multiplier, double start_multiplier);
   void set_master(scml_clock&);
   void set_master(sc_core::sc_in<bool>&);
   
 private:
   n_scml_clock::configurator* mConfigurator;
   n_scml_clock::on_off_control* mOnOffControl;
   mutable n_scml_clock::tick_callback_engine* mTickCallbackEngine;
   typedef std::vector<scml_clock_observer*> tObservers;
   tObservers mObservers;

   void init(n_scml_clock::configurator*);
   virtual void start_of_simulation();
   virtual void notify_parameters_updated();

};

SNPS_VP_API void collect_scml_clocks(std::vector<scml_clock*>& result);


typedef scml_clock cwr_clock;

#include "gate.h"

#endif
// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
