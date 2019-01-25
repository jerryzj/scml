/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include "scml_clock.h"
#include "master_configurator.h"
#include "divider_configurator.h"
#include "tick_callback_engine.h"
#include "scml2/tagged_message_macros.h"
#include <algorithm>

namespace n_scml_clock {
   typedef std::vector<scml_clock*> tClocks;
   tClocks* gClocks = 0;


}

scml_clock::scml_clock(const char* name)
  : engine(name)
  , mConfigurator(0), mOnOffControl(0), mTickCallbackEngine(0), mObservers()
{
   init(new n_scml_clock::master_configurator(this, sc_core::sc_time(1.0, true), sc_core::SC_ZERO_TIME, 0.5, true));
}

scml_clock::scml_clock()
: engine(sc_core::sc_gen_unique_name("scml_clock"))
  , mConfigurator(0), mOnOffControl(0), mTickCallbackEngine(0), mObservers()
{
   init(new n_scml_clock::master_configurator(this, sc_core::sc_time(1.0, true), sc_core::SC_ZERO_TIME, 0.5, true));
}

scml_clock::scml_clock(
   const char* name, const sc_core::sc_time& period, double duty_cycle, const sc_core::sc_time& start_time, bool posedge_first
) : engine(name)
  , mConfigurator(0), mOnOffControl(0), mTickCallbackEngine(0), mObservers()
{
   init(new n_scml_clock::master_configurator(this, period, start_time, duty_cycle, posedge_first));
}

scml_clock::scml_clock(
   const char* name, double period_v, sc_core::sc_time_unit period_tu, double duty_cycle,
   double start_time_v, sc_core::sc_time_unit start_time_tu, bool posedge_first
) : engine(name)
  , mConfigurator(0), mOnOffControl(0), mTickCallbackEngine(0), mObservers()
{
   init(new n_scml_clock::master_configurator(this, sc_core::sc_time(period_v, period_tu), sc_core::sc_time(start_time_v, start_time_tu), duty_cycle, posedge_first));
}

scml_clock::scml_clock(const char* name, double period_v, sc_core::sc_time_unit period_tu, double duty_cycle)
: engine(name)
  , mConfigurator(0), mOnOffControl(0), mTickCallbackEngine(0), mObservers()
{
   init(new n_scml_clock::master_configurator(this, sc_core::sc_time(period_v, period_tu), sc_core::SC_ZERO_TIME, duty_cycle, true));
}

scml_clock::scml_clock(const char* name, double period, double duty_cycle, double start_time, bool posedge_first)
: engine(name)
  , mConfigurator(0), mOnOffControl(0), mTickCallbackEngine(0), mObservers()
{
   init(new n_scml_clock::master_configurator(this, sc_core::sc_time(period, true), sc_core::sc_time(start_time, true), duty_cycle, posedge_first));
}

scml_clock::scml_clock(const char* name, scml_clock_if& clk, double period_multiplier, double start_multiplier)
: engine(name)
  , mConfigurator(0), mOnOffControl(0), mTickCallbackEngine(0), mObservers()
{
   init(new n_scml_clock::divider_configurator(this, clk.get_configurator(), period_multiplier, start_multiplier));
}

scml_clock::scml_clock(const char* name, sc_core::sc_in<bool>& clk, double period_multiplier, double start_multiplier, bool allow_stubbed)
: engine(name)
  , mConfigurator(0), mOnOffControl(0), mTickCallbackEngine(0), mObservers()
{
   n_scml_clock::divider_configurator* c =
      new n_scml_clock::divider_configurator(this, 0, period_multiplier, start_multiplier, allow_stubbed);
   c->set_master_port(&clk);
   init(c);
}

scml_clock::scml_clock(const char* name, scml_clock_if* clk, double period_multiplier, double start_multiplier)
: engine(name)
  , mConfigurator(0), mOnOffControl(0), mTickCallbackEngine(0), mObservers()
{
   init(new n_scml_clock::divider_configurator(this, clk!=0?clk->get_configurator():0, period_multiplier, start_multiplier));
}

void scml_clock::init(n_scml_clock::configurator* c) {

   if (n_scml_clock::gClocks == 0) {
      n_scml_clock::gClocks = new n_scml_clock::tClocks;
   }
   n_scml_clock::gClocks->push_back(this);

   mConfigurator = c;
   engine::set_configurator(mConfigurator);

}

bool scml_clock::is_master() const {
   return mConfigurator->is_master();
}

void scml_clock::set_master(scml_clock& clk) {
  n_scml_clock::divider_configurator* c = dynamic_cast<n_scml_clock::divider_configurator*>(mConfigurator);
  if (c == 0) {
     SCML2_LOG_ERROR(&clk, std::cerr, SCML_INVALID_API_USAGE) << "Error in scml_clock \"" << name() << "\": calling set_master() is invalid for a master clock!" << std::endl;
     abort();
  }
  c->set_master(clk.get_configurator());
}

void scml_clock::set_master(sc_core::sc_in<bool>& clk) {
  n_scml_clock::divider_configurator* c = dynamic_cast<n_scml_clock::divider_configurator*>(mConfigurator);
  if (c == 0) {
     SCML2_LOG_ERROR(this, std::cerr, SCML_INVALID_API_USAGE) << "Error in scml_clock \"" << name() << "\": calling set_master() is invalid for a master clock!" << std::endl;
     abort();
  }
  c->set_master_port(&clk);
}

scml_clock::~scml_clock() {
   n_scml_clock::gClocks->erase(std::find(n_scml_clock::gClocks->begin(), n_scml_clock::gClocks->end(), this));
   if (n_scml_clock::gClocks->empty()) {
      delete n_scml_clock::gClocks;
      n_scml_clock::gClocks = 0;
   }

   { // notify observers
      tObservers copy = mObservers;
      for (size_t i=0; i<copy.size(); ++i) {
	 copy[i]->handle_clock_deleted(this);
      }
   }

   delete mTickCallbackEngine;
   delete mConfigurator;
}

sc_core::sc_time scml_clock::get_period() const {
   return engine::get_period();
}

sc_core::sc_time scml_clock::get_start_time() const {
   return engine::get_offset();
}

bool scml_clock::get_posedge_first() const {
   return engine::get_posedge_first();
}

double scml_clock::get_duty_cycle() const {
   return engine::get_duty_cycle();
}

double scml_clock::get_period_multiplier() const {
  n_scml_clock::divider_configurator* c = dynamic_cast<n_scml_clock::divider_configurator*>(mConfigurator);
  if (c == 0) {
     SCML2_LOG_ERROR(this, std::cerr, SCML_INVALID_API_USAGE) << "Warning in scml_clock \"" << name() << "\": calling get_period_multiplier() is invalid for a master clock!" << std::endl;
     return 1.0;
  }
  return c->get_current_period_multiplier();
}

sc_core::sc_time scml_clock::get_next_edge_offset(bool pos_edge) const {
   return engine::get_next_edge_offset(pos_edge);
}

void scml_clock::set_period(const sc_core::sc_time& period) {
   SCML2_INFO(CONFIGURATION_INFO) << "scml_clock \"" << name() << "\": set period = " << period << std::endl;
   if (sc_get_curr_simcontext()->update_phase()) {
      SCML2_LOG_ERROR(this, std::cerr, SCML_INVALID_API_USAGE) << "Error in scml_clock \"" << name() << "\": set_period() must not be called from update phase!" << std::endl;
      abort();
   }
   mConfigurator->set_period(period);
}

void scml_clock::set_start_time(const sc_core::sc_time& start_time) {
   SCML2_INFO(CONFIGURATION_INFO) << "scml_clock \"" << name() << "\": configure start time = " << start_time << std::endl;
   mConfigurator->set_offset(start_time);
}

void scml_clock::set_posedge_first(bool posedge_first) {
   SCML2_INFO(CONFIGURATION_INFO) << "scml_clock \"" << name() << "\": configure " << (posedge_first? "POS":"NEG") << " edge first" << std::endl;
   mConfigurator->set_posedge_first(posedge_first);
}

void scml_clock::set_duty_cycle(double duty_cycle) {
   SCML2_INFO(CONFIGURATION_INFO) << "scml_clock \"" << name() << "\": set duty cycle = " << duty_cycle << std::endl;
   if (sc_get_curr_simcontext()->update_phase()) {
      SCML2_LOG_ERROR(this, std::cerr, SCML_INVALID_API_USAGE) << "Error in scml_clock \"" << name() << "\": set_duty_cycle() must not be called from update phase!" << std::endl;
      abort();
   }
   mConfigurator->set_duty_cycle(duty_cycle);
}
   
void scml_clock::set_period_multiplier(double m) {
   SCML2_INFO(CONFIGURATION_INFO) << "scml_clock \"" << name() << "\": set period multiplier = " << m << std::endl;
   if (sc_get_curr_simcontext()->update_phase()) {
      SCML2_LOG_ERROR(this, std::cerr, SCML_INVALID_API_USAGE) << "Error in scml_clock \"" << name() << "\": set_period_multiplier() must not be called from update phase!" << std::endl;
      abort();
   }
   mConfigurator->set_period_multiplier(m);
}

void scml_clock::enable() {
   SCML2_INFO(CONFIGURATION_INFO) << "scml_clock \"" << name() << "\": enabled" << std::endl;
   if (sc_get_curr_simcontext()->update_phase()) {
      SCML2_LOG_ERROR(this, std::cerr, SCML_INVALID_API_USAGE) << "Error in scml_clock \"" << name() << "\": enable() must not be called from update phase!" << std::endl;
      abort();
   }
   mConfigurator->set_enabled(true);
}

void scml_clock::disable() {
   SCML2_INFO(CONFIGURATION_INFO) << "scml_clock \"" << name() << "\": disabled" << std::endl;
   if (sc_get_curr_simcontext()->update_phase()) {
      SCML2_LOG_ERROR(this, std::cerr, SCML_INVALID_API_USAGE) << "Error in scml_clock \"" << name() << "\": disable() must not be called from update phase!" << std::endl;
      abort();
   }
   mConfigurator->set_enabled(false);
}

bool scml_clock::disabled() const {
   return !mConfigurator->is_engine_enabled();
}

bool scml_clock::running() const {
   return engine::is_running();
}
   
sc_dt::uint64 scml_clock::get_clock_count() const {
   return engine::get_clock_count();
}

inline n_scml_clock::tick_callback_engine* scml_clock::get_tick_callback_engine() const {
   if (mTickCallbackEngine == 0) {
      mTickCallbackEngine = new n_scml_clock::tick_callback_engine(
	 const_cast<scml_clock*>(this), engine::get_period(), engine::get_offset(), engine::is_running()
      );
   }
   return mTickCallbackEngine;
}


bool scml_clock::check_at_tick() const {
   return get_tick_callback_engine()->check_at_tick();
}

unsigned long long scml_clock::get_tick_count() const {
   return get_tick_callback_engine()->get_tick_count();
}

unsigned long long scml_clock::get_tick_count(const sc_core::sc_time& delay) const {
   return get_tick_callback_engine()->get_tick_count(delay);
}

sc_core::sc_time scml_clock::get_tick_time(long long clock_ticks_to_skip) const {
   return get_tick_callback_engine()->get_tick_time(clock_ticks_to_skip);
}

void scml_clock::get_next_tick_data(
   const sc_core::sc_time& delay, unsigned long long& count, sc_core::sc_time& time
) const {
   return get_tick_callback_engine()->get_next_tick_data(delay, count, time);
}

void scml_clock::start_of_simulation() {
   engine::handle_start_of_simulation(true);
}


void scml_clock::dump(std::ostream& os) const {
  os << " scml_clock : "           << name() << ":" << std::endl
     << "    start_time/offset : " << get_offset() << std::endl
     << "    period : "            << get_period() << std::endl
     << "    duty_cycle : "        << get_duty_cycle() << std::endl
     << "    posedge_first : "     << (get_posedge_first()? "true":"false") << std::endl
     << "    running : "           << (is_running()? "true":"false") << std::endl
     << "    optimized : "         << (!is_engine_on()? "true":"false") << std::endl;
}


bool scml_clock::register_observer(scml_clock_observer* o) {
   if (std::find(mObservers.begin(), mObservers.end(), o) != mObservers.end()) {
      return false;
   }
   mObservers.push_back(o);
   return true;
}

bool scml_clock::unregister_observer(scml_clock_observer* o) {
   for (tObservers::iterator i=mObservers.begin(); i!=mObservers.end(); ++i) {
      if (*i == o) {
	 mObservers.erase(i);
	 return true;
      }
   }
   return false;
}


void scml_clock::notify_parameters_updated() {
   if (mTickCallbackEngine != 0) {
      mTickCallbackEngine->update_parameters(engine::get_period(), engine::get_offset(), engine::is_running());
   }
   { // notify observers
      tObservers copy = mObservers;
      for (size_t i=0; i<copy.size(); ++i) {
	 copy[i]->handle_clock_parameters_updated(this);
      }
   }
}

void collect_scml_clocks(std::vector<scml_clock*>& result) {
   if (n_scml_clock::gClocks == 0) {
      result.clear();
   } else {
      result = *n_scml_clock::gClocks;
   }
}


scml_clock_if* scml2::get_scml_clock(sc_core::sc_in<bool>& clk, bool allow_stubbed) {
   sc_interface* clk_if = 
      clk.get_interface()
      ;
   if (clk_if == 0) {
     sc_core::sc_module* parent = dynamic_cast<sc_core::sc_module*>(clk.get_parent());
     (void)parent; // avoid compilation warning in build variants
     SCML2_ERROR_TO(parent, SCML_INVALID_API_USAGE)
       << "Error in get_scml_clock \"" << clk.name() << "\": the driving clock has not yet been bound to any channel!" << std::endl;
     abort();
   }
   scml_clock_if* clock = dynamic_cast<scml_clock_if*>(clk_if);
   if (clock == 0) {
      if (allow_stubbed && dynamic_cast<sc_core::sc_signal<bool>*>(clk_if)!=0) {
	 return 0;
      }
      sc_core::sc_module* parent = dynamic_cast<sc_core::sc_module*>(clk.get_parent());
     (void)parent; // avoid compilation warning in build variants
     SCML2_ERROR_TO(parent, SCML_INVALID_API_USAGE)
	<< "Error in get_scml_clock \"" << clk.name()
	<< "\": the driving clock port must be bound to a channel that implements scml_clock_if!"
	<< std::endl;
      abort();
   }
   return clock;
}

sc_core::sc_time scml2::get_period(sc_core::sc_in<bool>& clk) {
   return get_scml_clock(clk)->get_period();
}

#include <map>

namespace scml2 {

  typedef std::map<scml_clock_if*, scml_clock_parameter_change_control_if*> t_period_change_control_registry;
  t_period_change_control_registry* g_period_change_control_registry = 0;

  void bind_scml_clock_parameter_change_control(scml_clock_if* clk, scml_clock_parameter_change_control_if* d) {
    if (g_period_change_control_registry == 0) {
      g_period_change_control_registry = new t_period_change_control_registry;
    }
    (*g_period_change_control_registry)[clk] = d;
  }

  void unbind_scml_clock_parameter_change_control(scml_clock_if* clk) {
    g_period_change_control_registry->erase(clk);
    if (g_period_change_control_registry->empty()) {
      delete g_period_change_control_registry;
      g_period_change_control_registry = 0;
    }
  }

  scml_clock_parameter_change_control_if* get_scml_clock_parameter_change_control(scml_clock_if* clk) {
    if (g_period_change_control_registry == 0) {
      return 0;
    }
    t_period_change_control_registry::const_iterator ptr= g_period_change_control_registry->find(clk);
    if (ptr == g_period_change_control_registry->end()) {
      return 0;
    }
    return ptr->second;
  }

  scml_clock_parameter_change_control_if* get_scml_clock_parameter_change_control(sc_core::sc_in<bool>& clk_in_port) {
    scml_clock_if* clk = scml2::get_scml_clock(clk_in_port);
    if (clk == 0) {
       return 0;
    }
    return get_scml_clock_parameter_change_control(clk);
  }

}


// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}

