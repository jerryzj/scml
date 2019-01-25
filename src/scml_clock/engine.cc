/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include "engine.h"
#include "configurator.h"
#include <sysc/kernel/sc_spawn.h>
#include <sysc/kernel/sc_simcontext.h>
#include "scml2/tagged_message_macros.h"

namespace n_scml_clock {
   class engine_edge_callback {
    private:
      engine *mEngine;
    public:
      engine_edge_callback(engine* e) : mEngine(e) {}
      void operator() () { mEngine->handle_edge(); }
   };
}

n_scml_clock::engine::engine(const char* name)
: sc_core::sc_signal<bool,sc_core::SC_ONE_WRITER>(name), 
  mConfigurator(0), mClockCountOffset(0), mParametersChanged(false),
  mEngineOn(false), mEngineActive(false), mSimulationStarted(false)
{
   sc_core::sc_spawn_options o;
   o.spawn_method();
   o.dont_initialize();
   o.set_sensitivity(&mNextEdgeEvent);
   std::string n = basename();
   n += "_edge_action";
   sc_spawn(engine_edge_callback(this), sc_core::sc_gen_unique_name(n.c_str() ), &o);
   // coverity[uninit_member] uninitialized by intention: mDutyCycle, mInverted, mRunning
}

void n_scml_clock::engine::set_configurator(configurator* c) {
   assert(mConfigurator == 0);
   mConfigurator = c;
}

void n_scml_clock::engine::init(const sc_core::sc_time& period, const sc_core::sc_time& offset, double duty_cycle, bool posedge_first, bool running) {
   assert(!sc_core::sc_is_running());
   assert(!mEngineActive);

   if (mSimulationStarted && mRunning && mEngineOn) {
      mNextEdgeEvent.cancel();
   }
   
   mPeriod = period;
   mOffset = offset;
   mDutyCycle = duty_cycle;
   mInverted = !posedge_first;
   mRunning = running;

   notify_parameters_updated();

   if (mSimulationStarted) {
      handle_start_of_simulation(mEngineOn);
   }
}

void n_scml_clock::engine::end_of_elaboration() {
   mConfigurator->init();
}

void n_scml_clock::engine::handle_start_of_simulation(bool turn_engine_on) {
   if (!mSimulationStarted) {
      assert(!mEngineOn);
      assert(!mEngineActive);
      mSimulationStarted = true;
   }

   mFirstHalfDuration = mPeriod * (mInverted? 1.0-mDutyCycle : mDutyCycle);
   mSecondHalfDuration = mPeriod - mFirstHalfDuration;
   
   if (mFirstHalfDuration == sc_core::SC_ZERO_TIME) {
      SCML2_LOG_ERROR(this, std::cerr, CONFIGURATION_ERROR) << "Error in scml_clock '" << name() << "': " << (mInverted?"low":"high")
		<< " time is zero; increase the period or increase the duty cycle" << std::endl;
      abort();
   }
   if (mSecondHalfDuration == sc_core::SC_ZERO_TIME) {
      SCML2_LOG_ERROR(this, std::cerr, CONFIGURATION_ERROR) << "Error in scml_clock '" << name() << "': " << (mInverted?"high":"low")
		<< " time is zero; increase the period or decrease the duty cycle" << std::endl;
      abort();
   }

   mEngineOn = turn_engine_on;
   if (mRunning && mEngineOn) {
      mNextEdgeEvent.notify(mOffset);
      m_cur_val = mInverted;
   }
   if (mRunning) {
      SCML2_INFO(CONFIGURATION_INFO) << "scml_clock \"" << name() << "\": initial configuration: period=" << mPeriod << ", offset=" << mOffset << std::endl;
   } else {
      SCML2_INFO(CONFIGURATION_INFO) << "scml_clock \"" << name() << "\": initial configuration: stopped" << std::endl;
   }
}

sc_core::sc_time n_scml_clock::engine::get_next_edge_offset(bool pos_edge) const {
   // todo: handle disabled clock: what to return???
   sc_core::sc_time now = sc_core::sc_time_stamp();
   if (now < mOffset) {
      sc_core::sc_time delta = mOffset - now;
      if ((mInverted^pos_edge) == 0) {
	 delta += mFirstHalfDuration;
      }
      return delta;
   }

   sc_core::sc_time delta = mPeriod - sc_core::sc_time((now-mOffset).value()%mPeriod.value(), false);
   if ((mInverted^pos_edge) == 0) {
      delta += mFirstHalfDuration;
      if (delta > mPeriod) {
	 delta -= mPeriod;
      }
   }
   return delta;
}


const bool& n_scml_clock::engine::read() const {
   return m_cur_val;
}

sc_dt::uint64 n_scml_clock::engine::get_clock_count() const {
   sc_core::sc_time now = sc_core::sc_time_stamp();
   if (!mRunning || now<mOffset || !sc_core::sc_is_running()) {
      return mClockCountOffset;
   }
   return mClockCountOffset + (now-mOffset).value()/mPeriod.value() + 1;
}


void n_scml_clock::engine::trigger_parameter_change() {
   assert(sc_core::sc_is_running());
   if (mParametersChanged) {
      return;
   }
   mParametersChanged = true;
   request_update();
}

void n_scml_clock::engine::handle_edge() {
   if (mEngineActive) { // normal operation mode
      m_new_val = !m_cur_val;
   } else if (mRunning && sc_core::sc_time_stamp()==mOffset) { // 1st edge after engine was enabled
      mEngineActive = true;
      m_new_val = !mInverted;
   }
   request_update();
}

void n_scml_clock::engine::update() {
   bool at_edge = m_new_val != m_cur_val;
   bool first_edge = at_edge && (m_new_val^mInverted);

   if (mParametersChanged && !first_edge) {
      mParametersChanged = false;
      mClockCountOffset = get_clock_count();
      mConfigurator->get_next_parameters(mRunning, mPeriod, mDutyCycle, mOffset);
      if (mRunning) {
	 mFirstHalfDuration = mPeriod * (mInverted? 1.0-mDutyCycle : mDutyCycle);
	 mSecondHalfDuration = mPeriod - mFirstHalfDuration;
      }
      mEngineActive = false;
      if (m_cur_val == true) {
	 m_new_val = false;
      	 sc_core::sc_signal<bool,sc_core::SC_ONE_WRITER>::update();
      }
      mNextEdgeEvent.cancel();
      if (mRunning && mEngineOn) {
	 mNextEdgeEvent.notify(mOffset-sc_core::sc_time_stamp());
      }
      notify_parameters_updated();
      if (mRunning) {
         SCML2_INFO(CONFIGURATION_INFO) << "scml_clock \"" << name() << "\": configuration update: period=" << mPeriod << ", offset=" << mOffset << std::endl;
      } else {
         SCML2_INFO(CONFIGURATION_INFO) << "scml_clock \"" << name() << "\": configuration update: stopped" << std::endl;
      }
      return;
   }

   assert(at_edge);
   assert(mRunning);
   
   if (mEngineActive) {
      sc_core::sc_signal<bool,sc_core::SC_ONE_WRITER>::update();
   } else {
      m_cur_val = m_new_val;
   }      
   
   if (mParametersChanged) { // delayed by 1st edge
      mNextEdgeEvent.notify(sc_core::SC_ZERO_TIME);
      return;
   }
   
   if (mEngineOn) {
      assert(mEngineActive);
      mNextEdgeEvent.notify(first_edge? mFirstHalfDuration : mSecondHalfDuration);
   } else {
      mEngineActive = false;
   }
}

const sc_core::sc_event& n_scml_clock::engine::value_changed_event() const {
   if (m_change_event_p == 0) {
      sc_core::sc_signal<bool,sc_core::SC_ONE_WRITER>::value_changed_event();
   }
   return *m_change_event_p; 
}
  
const sc_core::sc_event& n_scml_clock::engine::posedge_event() const {
   if (m_posedge_event_p == 0) {
      sc_core::sc_signal<bool,sc_core::SC_ONE_WRITER>::posedge_event();
   }
   return *m_posedge_event_p; 
}
  
const sc_core::sc_event& n_scml_clock::engine::negedge_event() const {
   if (m_negedge_event_p == 0) {
      sc_core::sc_signal<bool,sc_core::SC_ONE_WRITER>::negedge_event();
   }
   return *m_negedge_event_p; 
}

void n_scml_clock::engine::register_port(sc_core::sc_port_base&, const char* if_typename) {
   if (std::string(if_typename) == typeid(sc_core::sc_signal_inout_if<bool>).name()) {
      SCML2_LOG_ERROR(this, std::cerr, SCML_INVALID_API_USAGE) << "Error in scml_clock '" << name() << "': attempted to bind instance to sc_inout or sc_out" << std::endl;
      abort();
   }
}

void n_scml_clock::engine::write(const bool&) {
   SCML2_LOG_ERROR(this, std::cerr, SCML_INVALID_API_USAGE) << "Error in scml_clock '" << name() << "': attempt to write the value of the instance" << std::endl;
   abort();
}

sc_core::sc_time n_scml_clock::engine::get_period() const {
   mConfigurator->init();
   return mPeriod;
}

sc_core::sc_time n_scml_clock::engine::get_offset() const {
   mConfigurator->init();
   return mOffset;
}

double n_scml_clock::engine::get_duty_cycle() const {
   mConfigurator->init();
   return mDutyCycle;
}

bool n_scml_clock::engine::get_posedge_first() const {
   mConfigurator->init();
   return !mInverted;
}

bool n_scml_clock::engine::is_running() const {
   mConfigurator->init();
   return mRunning;
}

// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
