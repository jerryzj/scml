/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#ifndef SCML_CLOCK_DIVIDER_CONFIGURATOR_H
#define SCML_CLOCK_DIVIDER_CONFIGURATOR_H

#include "configurator.h"
#include <sysc/communication/sc_signal_ports.h>

namespace n_scml_clock {

   class engine;
   
   class divider_configurator : public configurator, protected configurator::child {
    public:
      divider_configurator(engine* e, configurator* master, double period_multiplier, double start_multiplier, bool allow_stubbed=false);
      virtual ~divider_configurator();

      virtual bool is_master() const { return false; }
      void set_master(configurator*);
      void set_master_port(sc_core::sc_in<bool>*);
      
      // by now only used during construction
      virtual sc_core::sc_time get_period() const;
      virtual sc_core::sc_time get_offset() const;
      virtual double get_duty_cycle() const;
      virtual bool get_posedge_first() const { return mPosedgeFirst; }
      virtual bool is_running() const;
      virtual bool is_enabled() const;

      double get_current_period_multiplier() const { return mCurrentPeriodMultiplier; }
      
      virtual void set_period(const sc_core::sc_time&);
      virtual void set_offset(const sc_core::sc_time&);
      virtual void set_posedge_first(bool);
      virtual void set_duty_cycle(double);
      virtual void set_period_multiplier(double);
      virtual void set_enabled(bool);

      
      virtual void get_next_parameters(bool& running, sc_core::sc_time& period, double& duty_cycle, sc_core::sc_time& offset);
      virtual sc_core::sc_time get_next_period_start_time() const;
    protected:
      configurator* mMaster;
      bool mEnabled;
      double mPeriodMultiplier;
      double mStartMultiplier;
      bool mUseDutyCycleFromMaster;
      double mDutyCycle; // only used if mUseDutyCycleFromMaster==false
      bool mPosedgeFirst;
      const bool mAllowStubbed;
      bool mInitialized;
      int mRemainingMasterPeriods;

      double mCurrentPeriodMultiplier; // the currenly active setting as the engine itself only holds the effective period
      
      void handle_parameters_changed();
      
      virtual void init();
      virtual void handle_parent_parameters_changed(configurator*);
      virtual void handle_parent_deleted(configurator*);

      sc_core::sc_in<bool>* mClkInPort; // only in case that we get out master via an in port
   };

}

#endif
// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
