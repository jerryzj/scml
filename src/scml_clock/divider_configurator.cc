/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include "divider_configurator.h"
#include "engine.h"
#include "scml2/tagged_message_macros.h"
#include "scml_clock_if.h"
#include <sysc/kernel/sc_simcontext.h>

n_scml_clock::divider_configurator::divider_configurator(
   engine* e, configurator* master, double period_multiplier, double start_multiplier, bool allow_stubbed
) : configurator(e, true), mMaster(master), mEnabled(true), mPeriodMultiplier(period_multiplier), mStartMultiplier(start_multiplier),
    mUseDutyCycleFromMaster(true), mPosedgeFirst(true), mAllowStubbed(allow_stubbed), mInitialized(false), mRemainingMasterPeriods(-1), mClkInPort(0)
{
   assert(!sc_core::sc_is_running());

   if (mStartMultiplier < 0.0) {
      SCML2_LOG_ERROR(mEngine, std::cerr, CONFIGURATION_ERROR) << "Error in scml_clock '" << mEngine->name() << "': start multiplier must be positive" << std::endl;
      abort();
   }

   mCurrentPeriodMultiplier = mPeriodMultiplier;
   if (mMaster != 0) {
      mMaster->register_child(this);
   }
   handle_parameters_changed();
}

n_scml_clock::divider_configurator::~divider_configurator() {
   if (mMaster != 0) {
      mMaster->unregister_child(this);
   }
}

void n_scml_clock::divider_configurator::set_master(configurator* m) {
   assert(mMaster == 0);
   assert(mClkInPort == 0);
   mMaster = m;
}

void n_scml_clock::divider_configurator::set_master_port(sc_core::sc_in<bool>* p) {
   assert(mMaster == 0);
   assert(mClkInPort == 0);
   mClkInPort = p;
}

sc_core::sc_time n_scml_clock::divider_configurator::get_period() const {
   if (mMaster == 0) {
      static sc_core::sc_time dummy_period(1, sc_core::SC_NS);
      return dummy_period;
   }
   return mPeriodMultiplier * mMaster->get_period();
}

sc_core::sc_time n_scml_clock::divider_configurator::get_offset() const {
   if (mMaster == 0) {
      return sc_core::SC_ZERO_TIME;
   }
   return mMaster->get_offset() + mStartMultiplier*mMaster->get_period();
}

double n_scml_clock::divider_configurator::get_duty_cycle() const {
   if (!mUseDutyCycleFromMaster) {
      return mDutyCycle;
   }
   if (mMaster == 0) {
      return 0.5;
   }
   return mMaster->get_duty_cycle();
}

bool n_scml_clock::divider_configurator::is_running() const {
   if (mMaster == 0) {
      return false;
   }
   return mEnabled && mMaster->is_running();
}

bool n_scml_clock::divider_configurator::is_enabled() const {
   return mEnabled;
}

void n_scml_clock::divider_configurator::set_period(const sc_core::sc_time&) {
   SCML2_LOG_ERROR(mEngine, std::cerr, CONFIGURATION_ERROR) << "Error in scml_clock \"" << mEngine->name() << "\": calling set_period() is invalid for a derived clock! You may consider to use set_period_multiplier() instead. " << std::endl;
   abort();
}

void n_scml_clock::divider_configurator::set_offset(const sc_core::sc_time&) {
   SCML2_LOG_ERROR(mEngine, std::cerr, CONFIGURATION_ERROR) << "Error in scml_clock \"" << mEngine->name() << "\": calling set_offset() is invalid for a derived clock!" << std::endl;
   abort();
}

void n_scml_clock::divider_configurator::set_posedge_first(bool posedge_first) {
   if (sc_core::sc_is_running()) {
      SCML2_LOG_ERROR(mEngine, std::cerr, CONFIGURATION_ERROR) << "Error in scml_clock \"" << mEngine->name() << "\": set_posedge_first() called after construction!" << std::endl;
      abort();
   }
   mPosedgeFirst = posedge_first;
   handle_parameters_changed();
}

void n_scml_clock::divider_configurator::set_duty_cycle(double duty_cycle) {
   mUseDutyCycleFromMaster = false;
   mDutyCycle = duty_cycle;
   if (sc_core::sc_is_running()) {
      mRemainingMasterPeriods = mEngineEnabled? (int)mPeriodMultiplier-1 : 0;
   }
   handle_parameters_changed();
}

void n_scml_clock::divider_configurator::set_period_multiplier(double m) {
   mPeriodMultiplier = m;
   if (sc_core::sc_is_running()) {
      mRemainingMasterPeriods = mEngineEnabled? (int)m-1 : 0;
   }
   handle_parameters_changed();
}

void n_scml_clock::divider_configurator::set_enabled(bool enabled) {
   mEnabled = enabled;
   if (sc_core::sc_is_running()) {
      mRemainingMasterPeriods = mEngineEnabled? (int)mPeriodMultiplier-1 : 0;
   }
   handle_parameters_changed();
}


void n_scml_clock::divider_configurator::get_next_parameters(bool& running, sc_core::sc_time& period, double& duty_cycle, sc_core::sc_time& offset) {
   assert(sc_core::sc_is_running());

   if (mEngine->is_running() && mRemainingMasterPeriods==-1) {
      sc_core::sc_time curr_next_edge_delay = mEngine->get_next_edge_offset(mEngine->get_posedge_first());
      sc_core::sc_time curr_period = mEngine->get_period();
      sc_dt::uint64 already_consumed_period = curr_period.value() - curr_next_edge_delay.value();
      int already_consumed_master_ticks = (int)(already_consumed_period/(double)curr_period.value()*mCurrentPeriodMultiplier);
      mRemainingMasterPeriods = (int)mCurrentPeriodMultiplier - already_consumed_master_ticks - 1;
   }
   
   running = is_running();
   period = get_period();
   duty_cycle = get_duty_cycle();
   if (running) {
      offset = mMaster->get_next_period_start_time();
      if (mRemainingMasterPeriods > 0) {
	 offset += mRemainingMasterPeriods*mMaster->get_period();
      }
      mRemainingMasterPeriods = -1;
   } else {
      offset = sc_core::sc_time_stamp();
   }
   mEngineEnabled = mEnabled;
   mCurrentPeriodMultiplier = mPeriodMultiplier;
}

sc_core::sc_time n_scml_clock::divider_configurator::get_next_period_start_time() const {
   assert(is_running());
   assert(mMaster != 0);

   if (!mEngine->is_parameter_change_triggered()) {
      return sc_core::sc_time_stamp() + mEngine->get_next_edge_offset(mEngine->get_posedge_first());
   }
   sc_core::sc_time t = mMaster->get_next_period_start_time();
   if (mRemainingMasterPeriods > 0) {
      t += mRemainingMasterPeriods*mMaster->get_period();
   }
   return t;
}

void n_scml_clock::divider_configurator::init() {
   if (mInitialized) {
      return;
   }
   mInitialized = true;
   if (mClkInPort == 0) {
      assert(mMaster != 0);
      mMaster->init();
      return;
   }
   assert(mMaster == 0);
   scml_clock_if* master_clock = dynamic_cast<scml_clock_if*>(
      mClkInPort->get_interface()
   );
   if (master_clock != 0) {
      mMaster = master_clock->get_configurator();
      mMaster->register_child(this);
      mMaster->init();
   } else {
      bool stubbed = (dynamic_cast<sc_core::sc_signal<bool>*>(
			 mClkInPort->get_interface()
		      ) != 0);
      if (!mAllowStubbed || !stubbed) {
	 SCML2_LOG_ERROR(mEngine, std::cerr, CONFIGURATION_ERROR) << "Error in scml_clock '" << mEngine->name() << "': the attached master clock '" << mClkInPort->name()
		   << "' is no scml_clock!" << std::endl;
	 abort();
      }
   }
   handle_parameters_changed();
}
      
void n_scml_clock::divider_configurator::handle_parameters_changed() {
   if (mPeriodMultiplier <= 0.0) {
      SCML2_LOG_ERROR(mEngine, std::cerr, CONFIGURATION_ERROR) << "Error in scml_clock '" << mEngine->name() << "': period multiplier must be positive and non-zero" << std::endl;
      abort();
   }
   if (!mUseDutyCycleFromMaster && (mDutyCycle<=0.0 || 1.0<=mDutyCycle)) {
      SCML2_LOG_ERROR(mEngine, std::cerr, CONFIGURATION_ERROR) << "Warning in scml_clock \"" << mEngine->name() << "\": duty cylce " << mDutyCycle
		<< " is out of range (0..1). Changing to 0.5" << std::endl;
      mDutyCycle = 0.5;
   }
   if (!sc_core::sc_is_running()) {
      mEngineEnabled = mEnabled;
      mCurrentPeriodMultiplier = mPeriodMultiplier;
      mEngine->init(get_period(), get_offset(), get_duty_cycle(), get_posedge_first(), is_running());
   } else {
      mEngine->trigger_parameter_change();
   }
   notify_childen();
}

void n_scml_clock::divider_configurator::handle_parent_parameters_changed(configurator* c) {
   assert(c == mMaster); (void)c; // avoid compiler warning about unused variable
   handle_parameters_changed();
}

void n_scml_clock::divider_configurator::handle_parent_deleted(configurator* c) {
   assert(c == mMaster); (void)c; // avoid compiler warning about unused variable
   mMaster = 0;
}

// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
