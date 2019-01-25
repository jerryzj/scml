/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include "master_configurator.h"
#include "engine.h"

#include <sysc/kernel/sc_simcontext.h>
#include "scml2/tagged_message_macros.h"


n_scml_clock::master_configurator::master_configurator(
   engine* e, const sc_core::sc_time& period, const sc_core::sc_time& offset, double duty_cycle, bool posedge_first
) : configurator(e, true), mEnabled(true), mPeriod(period), mDutyCycle(duty_cycle), mPosedgeFirst(posedge_first), mOffset(offset)
{
   check_period(mPeriod);
   check_duty_cycle(mDutyCycle);
   handle_parameters_changed();
   assert(!sc_core::sc_is_running());
}

void n_scml_clock::master_configurator::set_period(const sc_core::sc_time& period) {
   mPeriod = period;
   check_period(mPeriod);
   handle_parameters_changed();
}

void n_scml_clock::master_configurator::set_offset(const sc_core::sc_time& offset) {
   if (sc_core::sc_is_running()) {
      SCML2_LOG_ERROR(mEngine, std::cerr, SCML_INVALID_API_USAGE) << "Error in scml_clock \"" << mEngine->name() << "\": set_offset() called after construction!" << std::endl;
      abort();
   }
   mOffset = offset;
   handle_parameters_changed();
}

void n_scml_clock::master_configurator::set_duty_cycle(double duty_cycle) {
   mDutyCycle  = duty_cycle;
   check_duty_cycle(mDutyCycle);
   handle_parameters_changed();
}

void n_scml_clock::master_configurator::set_posedge_first(bool posedge_first) {
   if (sc_core::sc_is_running()) {
      SCML2_LOG_ERROR(mEngine, std::cerr, SCML_INVALID_API_USAGE) << "Error in scml_clock \"" << mEngine->name() << "\": set_posedge_first() called after construction!" << std::endl;
      abort();
   }
   mPosedgeFirst = posedge_first;
   handle_parameters_changed();
}

void n_scml_clock::master_configurator::set_period_multiplier(double) {
   SCML2_LOG_ERROR(mEngine, std::cerr, SCML_INVALID_API_USAGE) << "Error in scml_clock \"" << mEngine->name() << "\": calling set_period_multiplier() is invalid for a master clock!" << std::endl;
   abort();
}

void n_scml_clock::master_configurator::set_enabled(bool enabled) {
   mEnabled  = enabled;
   handle_parameters_changed();
}


void n_scml_clock::master_configurator::check_period(const sc_core::sc_time& period) const {
   if (period == sc_core::SC_ZERO_TIME) {
      SCML2_LOG_ERROR(mEngine, std::cerr, CONFIGURATION_ERROR) << "Error in scml_clock \"" << mEngine->name()
                                                               << "\": Period is zero. Increase the period!" << std::endl;
      abort();
   }
}

void n_scml_clock::master_configurator::check_duty_cycle(double& duty_cycle) const {
   if (duty_cycle<=0.0 || 1.0<=duty_cycle) {
      SCML2_LOG_ERROR(mEngine, std::cerr, CONFIGURATION_ERROR) << "Warning in scml_clock \"" << mEngine->name() << "\": duty cylce " << duty_cycle
		<< " is out of range (0..1). Changing to 0.5" << std::endl;
      duty_cycle = 0.5;
   }
}

void n_scml_clock::master_configurator::handle_parameters_changed() {
   if (!sc_core::sc_is_running()) {
      mEngineEnabled = mEnabled;
      mEngine->init(mPeriod, mOffset, mDutyCycle, mPosedgeFirst, mEnabled);
   } else {
      mOffset = sc_core::sc_time_stamp();
      if (mEnabled) {
	 mOffset += mPeriod;
      }
      mEngine->trigger_parameter_change();
   }
   notify_childen();
}

void n_scml_clock::master_configurator::get_next_parameters(bool& running, sc_core::sc_time& period, double& duty_cycle, sc_core::sc_time& offset) {
   running = mEnabled;
   period = mPeriod;
   duty_cycle = mDutyCycle;
   offset = mOffset;
   mEngineEnabled = mEnabled;
}

sc_core::sc_time n_scml_clock::master_configurator::get_next_period_start_time() const {
   assert(mEnabled);

   sc_core::sc_time now = sc_core::sc_time_stamp();
   if (now < mOffset) {
      return mOffset;
   }
   return now - sc_core::sc_time((now-mOffset).value()%mPeriod.value(), false) + mPeriod;
}

// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
