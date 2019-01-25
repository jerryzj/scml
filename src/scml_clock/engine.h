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
#ifndef SCML_CLOCK_ENGINE_H
#define SCML_CLOCK_ENGINE_H

#include <sysc/communication/sc_signal.h>
#include <scml2_logging/snps_vp_dll.h>

namespace n_scml_clock {
   
   class engine_edge_callback;
   class configurator;
   class master_configurator;
   class divider_configurator;

   class SNPS_VP_API engine : public sc_core::sc_signal<bool,sc_core::SC_ONE_WRITER>
   {
    public:
      engine(const char* name);
      virtual void register_port(sc_core::sc_port_base&, const char*);
      virtual const sc_core::sc_event& default_event() const { return value_changed_event(); }
      virtual const sc_core::sc_event& value_changed_event() const;
      virtual const sc_core::sc_event& posedge_event() const;
      virtual const sc_core::sc_event& negedge_event() const;
      virtual const bool& read() const;
      virtual void write(const bool&);
      sc_dt::uint64 get_clock_count() const;
      sc_core::sc_time get_next_edge_offset(bool pos_edge) const;
      bool is_engine_on() const { return mEngineOn; } // it's on if someone is interested in our events
    protected:
      void set_configurator(configurator*);
      void init(const sc_core::sc_time& period, const sc_core::sc_time& offset, double duty_cycle, bool posedge_first, bool running);
      virtual void end_of_elaboration();
      virtual void handle_start_of_simulation(bool turn_engine_on);

      sc_core::sc_time get_period() const;
      sc_core::sc_time get_offset() const;
      double get_duty_cycle() const;
      bool get_posedge_first() const;

      bool is_running() const;
      
      void handle_edge();
      virtual void update();
      void trigger_parameter_change();
      bool is_parameter_change_triggered() { return mParametersChanged; }
      virtual void notify_parameters_updated() =0;

      configurator* get_configurator() const { return mConfigurator; }
    private:
      // config parameters
      configurator* mConfigurator;

      // active parameters
      sc_core::sc_time mOffset;
      sc_core::sc_time mPeriod;
      double mDutyCycle;
      bool mInverted;
      bool mRunning;

      sc_dt::uint64 mClockCountOffset;
      
      bool mParametersChanged;
      
      // operation parameters
      sc_core::sc_time mFirstHalfDuration;
      sc_core::sc_time mSecondHalfDuration;
      sc_core::sc_event mNextEdgeEvent;

      bool mEngineOn; // requested by on_off_control (ensureEngineOn/ensureEngineOff)
      bool mEngineActive; // we have an active period (events scheduled)

      bool mSimulationStarted; // handle_start_of_simulation() already called
      
      friend class engine_edge_callback;
      friend class configurator;
      friend class master_configurator;
      friend class divider_configurator;
      friend class pin_divider_configurator;
   };

}


#endif
// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
