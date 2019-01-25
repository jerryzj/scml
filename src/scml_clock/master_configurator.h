/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#ifndef SCML_CLOCK_MASTER_CONFIGURATOR_H
#define SCML_CLOCK_MASTER_CONFIGURATOR_H

#include "configurator.h"

namespace n_scml_clock {

   class master_configurator : public configurator
   {
    public:
      master_configurator(engine*, const sc_core::sc_time& period, const sc_core::sc_time& offset, double duty_cycle, bool posedge_first);
      virtual ~master_configurator() {}

      virtual bool is_master() const { return true; }

      virtual sc_core::sc_time get_period() const {
         return mPeriod;
      }
      virtual sc_core::sc_time get_offset() const  {
         return mOffset;
      }
      virtual double get_duty_cycle() const  {
         return mDutyCycle;
      }
      virtual bool get_posedge_first() const  {
         return mPosedgeFirst;
      }
      virtual bool is_running() const  {
         return mEnabled;
      }
      virtual bool is_enabled() const  {
         return mEnabled;
      }

      virtual void set_period(const sc_core::sc_time&);
      virtual void set_offset(const sc_core::sc_time&);
      virtual void set_posedge_first(bool);
      virtual void set_duty_cycle(double);
      virtual void set_period_multiplier(double);
      virtual void set_enabled(bool);


      virtual void get_next_parameters(bool& running, sc_core::sc_time& period, double& duty_cycle, sc_core::sc_time& offset);
      sc_core::sc_time get_next_period_start_time() const;

    private:
      bool mEnabled;
      sc_core::sc_time mPeriod;
      double mDutyCycle;
      bool mPosedgeFirst;
      sc_core::sc_time mOffset;

      void check_period(const sc_core::sc_time& period) const;
      void check_duty_cycle(double& duty_cycle) const;
      void handle_parameters_changed();


   };

}

#endif
// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
