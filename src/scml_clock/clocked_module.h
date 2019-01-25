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
#ifndef SCML_CLOCK_CLOCKED_MODULE_H
#define SCML_CLOCK_CLOCKED_MODULE_H

#include "tick_callback_engine.h"
#include <systemc>
#include <scml2_logging/snps_vp_dll.h>

class scml_clock_if;

namespace scml2 {
   

   /** The class clocked_module is the low level user API for SCML clock
    *  tick callbacks. A model can receive clock tick callbacks by inheriting
    *  from clocked_module and reimplementing the virtual method
    *  handle_clock_tick(). A clocked_module has a fixed association to one
    *  SCML clock, which is established by the clock argument in the
    *  constructor, or by calling set_clock(). Once a clock has been
    *  assigned to the clocked_module, a clock tick callback for a certain
    *  tick count can be requested by the method
    *  request_clock_trigger(). It is possible to re-trigger or cancel a
    *  pending request. */
   class SNPS_VP_API clocked_module {
    public:
      /** Constructs a clocked_module and associates it with the provided SCML clock
       *  CLOCK. */
      clocked_module(scml_clock_if* clock=0);
      /** Constructs a clocked_module and associates it with the SCML
       *  clock, which is bound to the provided port P. */
      clocked_module(sc_core::sc_in<bool>& p);
      /** Destructs the clocked_module. If there is still a pending clock
       *  trigger request during destruction, it will automatically be
       *  canceled. */
      virtual ~clocked_module();

      /** Associates the clocked_module with the provided SCML clock
       *  CLOCK. */
      void set_clock(scml_clock_if* clock);
      /** Associates the clocked_module with the SCML clock, which is bound
       *  to the provided port P. */
      void set_clock(sc_core::sc_in<bool>& p);
      /** Returns the associated SCML clock. */
      scml_clock_if* get_clock() const;

      /** When a requested clock trigger request expires, the SCML clock
       *  calls the method handle_clock_tick(). The call happens from
       *  within the context of an SC_METHOD that is owned by the clock
       *  object.
       *  WARNING: You must not call next_trigger() from within a clock
       *  callback, since this will break the SCML clock mechanism and
       *  result in a fatal misbehavior of the clock system. */
      virtual void handle_clock_tick() = 0;

      /** Schedules a handle_clock_tick() trigger for a future clock
       *  tick. The argument CLOCK_TICKS_TO_SKIP defines the number of
       *  ticks that shall be skipped from now. Can be called at any
       *  SystemC time.
       *
       *  If called while a previous clock trigger request is still
       *  pending, then the new request is ignored if it targets the same
       *  or a later clock tick. If it targets an earlier clock tick, then
       *  the previous request is canceled, and the request is scheduled
       *  for the new earlier tick.
       *
       *  If a clock trigger was requested, then the
       *  requested/scheduled clock tick can be retrieved by a call to
       *  method get_scheduled_clock_tick().
       *
       *  For example, after a call of request_clock_trigger(0), the
       *  clock tick count returned by get_scheduled_clock_tick() will
       *  be one larger than the current clock tick count of the
       *  associated SCML clock.*/
      void request_clock_trigger(long long clock_ticks_to_skip);
      /** Schedules a handle_clock_tick() trigger for a future clock tick
       *  after the time defined by the argument DELAY. The argument DELAY
       *  is a SystemC time that is interpreted relative to the current
       *  SystemC time sc_time_stamp(). This method will usually be used by
       *  TLM2 models with temporal decoupling in order to synchronize to
       *  the next clock tick after the given local time argument.
       *
       *  If called while a previous clock trigger request is still
       *  pending, then the new request is ignored if it targets the same
       *  or a later clock tick. If it targets an earlier clock tick, then
       *  the previous request is canceled, and the request is scheduled for
       *  the new earlier tick. */
      void request_clock_trigger(const sc_core::sc_time& delay);
      /** Returns true if a clock trigger was requested and is still
       *  pending. */
      bool is_clock_trigger_requested() const { return mScheduledTick!=~0ull; }
      /** Cancels a pending clock trigger request. It does nothing if no
       *  clock trigger request is currently pending. During the processing
       *  of a clock tick (i.e. from a handle_clock_tick() call) it is not
       *  permitted to cancel a clock trigger request that was scheduled
       *  for the currently processed clock tick. */
      void cancel_clock_trigger();
      /** In case of a pending clock trigger this method returns the clock
       *  tick for which the trigger was requested/scheduled. */
      unsigned long long get_scheduled_clock_tick() const { return mScheduledTick; }

    private:
      n_scml_clock::tick_callback_engine* mEngine;
      unsigned long long mScheduledTick;
      clocked_module*	mNextRequest;


      friend class n_scml_clock::tick_callback_engine;
   };
   
}

inline void scml2::clocked_module::request_clock_trigger(long long clock_ticks_to_skip) {
   mEngine->request_trigger(this, clock_ticks_to_skip);
}

inline void scml2::clocked_module::request_clock_trigger(const sc_core::sc_time& delay) {
   mEngine->request_trigger(this, delay);
}

inline void scml2::clocked_module::cancel_clock_trigger() {
   mEngine->cancel_clock_trigger(this);
}

#endif
// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
