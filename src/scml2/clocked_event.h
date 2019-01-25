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
#ifndef SCML2_CLOCKED_EVENT_H
#define SCML2_CLOCKED_EVENT_H

#include "clocked_module.h"
#include <scml2_logging/snps_vp_dll.h>

namespace scml2 {
   
   /** A convenience class that allows a SystemC method or thread to wait
    *  until a certain clock tick happens. */
   class SNPS_VP_API clocked_event : public sc_core::sc_object, public clocked_module {
    public:
      /** Construct a clocked_event of name NAME that is associated with
       *  the SCML2 clock object CLOCK. */ 
      clocked_event(const std::string& name, scml_clock_if* clock=0)
      : sc_core::sc_object(name.c_str()), clocked_module(clock), mEvent()
      {}
      /** Construct a clocked_event of name NAME that is associated with
       *  the SCML clock object, which is bound to the provided port P. */ 
      clocked_event(const std::string& name, sc_core::sc_in<bool>& p)
      : sc_core::sc_object(name.c_str()), clocked_module(p), mEvent()
      {}

      /** Destructs the clocked_event. If there is still a trigger pending,
       *  it will automatically be canceled. If a thread or method is
       *  waiting for a clock tick, it will never wake up or be activated
       *  again. */
      virtual ~clocked_event() {}
      
      /** Associates the clocked_event with the provided SCML2 clock
       *  CLOCK. */
      void set_clock(scml_clock_if* clock) {
	 clocked_module::set_clock(clock);
      }
      /** Associates the clocked_event with the SCML clock, which is bound
       *  to the provided port P. */
      void set_clock(sc_core::sc_in<bool>& p) {
	 clocked_module::set_clock(p);
      }
      /** Returns the associated SCML2 clock. */
      scml_clock_if* get_clock() const {
	 return clocked_module::get_clock();
      }
      
      /** Schedules a trigger for a future clock tick, which will wakeup a
       *  SystemC thread (which called wait...(), or which re-activates a
       *  SystemC methods (which called next_trigger()). The argument
       *  CLOCK_TICKS_TO_SKIP defines the number of ticks that shall be
       *  skipped from now. It can be called at any SystemC time.
       *
       *  If called while a previous trigger request is still pending, then
       *  the new request is ignored if it targets the same or a later
       *  clock tick. If it targets an earlier clock tick, then the
       *  previous request is canceled, and the request is scheduled for
       *  the new earlier tick.
       *  
       *  If a trigger was requested, then the requested/scheduled
       *  clock tick can be retrieved by a call to method
       *  get_scheduled_clock_tick().
       *
       *  For example, after a call of request_clock_trigger(0), the
       *  clock tick count returned by get_scheduled_clock_tick() will
       *  be one larger than the current clock tick count of the
       *  associated SCML clock.*/
      void request_trigger(long long clock_ticks_to_skip) {
	 request_clock_trigger(clock_ticks_to_skip);
      }
      /** Schedules a trigger for a future clock tick after the time
       *  defined by the argument DELAY. The argument DELAY is a SystemC
       *  time that is interpreted relative to the current SystemC time
       *  sc_time_stamp(). This method will usually be used by TLM2 models
       *  with temporal decoupling in order to synchronize to the next
       *  clock tick after the given local time argument.
       *
       *  If called while a previous trigger request is still pending, then
       *  the new request is ignored if it targets the same or a later
       *  clock tick. If it targets an earlier clock tick, then the
       *  previous request is canceled, and the request is scheduled for
       *  the new earlier tick. */
      void request_trigger(sc_core::sc_time delay) {
	 request_clock_trigger(delay);
      }
      /** Returns true if a trigger was requested and is still pending. */
      bool is_trigger_requested() const {
	 return is_clock_trigger_requested();
      }
      /** Cancels a pending trigger request. It does nothing if no trigger
       *  is currently pending. During the processing of a clock tick
       *  (i.e. from a callback, or a handle_clock_tick() call) it is not
       *  permitted to cancel a trigger request that was scheduled for the
       *  currently processed clock tick. */
      void cancel_trigger() {
	 cancel_clock_trigger();
      }
      
      /** Blocks the calling SystemC thread until a clocked trigger
       *  happens. The clocked trigger is requested by a call to the method
       *  request_trigger(ticks/delay). The blocked thread wakes up when
       *  the requested clock tick happens. It must only be called from the
       *  context of a SystemC thread.*/
      void waitfor_trigger() {
	 sc_core::wait(mEvent);
      }
      /** Blocks the calling SystemC thread until the specified clock tick
       *  happened. The clock tick is specified by the argument
       *  CLOCK_TICKS_TO_SKIP, which defines the number of ticks that shall
       *  be skipped from now. It can be called at any SystemC time, but
       *  only from the context of a SystemC thread.
       *
       *  It is possible to wake up the thread at an earlier time by
       *  calling request_trigger(ticks/delay) with an argument that
       *  specifies an earlier clock tick. In order to wake up the thread
       *  at a later time, it is first necessary to cancel the pending
       *  trigger with a call to cancel_trigger() */
      void wait(long long clock_ticks_to_skip) {
	 request_trigger(clock_ticks_to_skip);
	 waitfor_trigger();
      }
      /** Blocks the calling SystemC thread until the specified clock tick
       *  happened. The clock tick is specified by the argument DELAY,
       *  which is a SystemC time that is interpreted relative to the
       *  current SystemC time sc_time_stamp(). This method will usually be
       *  used by TLM2 models with temporal decoupling in order to
       *  synchronize to the next clock tick after the given local time
       *  argument. It can be called at any SystemC time, but only from the
       *  context of a SystemC thread.
       *
       *  It is possible to wake up the thread at an earlier time by
       *  calling request_trigger(ticks/delay) with an argument that
       *  specifies an earlier clock tick. In order to wake up the thread
       *  at a later time, it is first necessary to cancel the pending
       *  trigger with a call to cancel_trigger() */
      void wait(sc_core::sc_time delay) {
	 request_trigger(delay);
	 waitfor_trigger();
      }

      /** Temporary overrides the static sensitivity list of the calling
       *  SystemC method. The method will be reactivated when a certain
       *  clock tick happens. With this version of the next_trigger()
       *  method, the triggering clock tick is left undefined. It must be
       *  scheduled by calling the request_trigger(ticks/delay) method. */
      void next_trigger() {
	 sc_core::next_trigger(mEvent);
      }
      /** Temporary overrides the static sensitivity list of the calling
       *  SystemC method. The method will be reactivated when specified
       *  clock tick happens. The clock tick is specified by the argument
       *  CLOCK_TICKS_TO_SKIP, which defines the number of ticks that shall
       *  be skipped from now. It can be called at any SystemC time, but
       *  only from the context of a SystemC method.
       *
       *  It is possible to reactivate the method at an earlier time by
       *  calling request_trigger(ticks/delay) with an argument that
       *  specifies an earlier clock tick. In order to wake up the method
       *  at a later time, it is first necessary to cancel the pending
       *  trigger with a call to cancel_trigger() */
      void next_trigger(long long clock_ticks_to_skip) {
	 request_trigger(clock_ticks_to_skip);
	 next_trigger();
      }
      /** Temporary overrides the static sensitivity list of the calling
       *  SystemC method. The method will be reactivated when specified
       *  clock tick happens. The clock tick is specified by the argument
       *  DELAY, which is a SystemC time that is interpreted relative to
       *  the current SystemC time sc_time_stamp(). This method will
       *  usually be used by TLM2 models with temporal decoupling in order
       *  to synchronize to the next clock tick after the given local time
       *  argument. It can be called at any SystemC time, but only from the
       *  context of a SystemC method.
       *
       *  It is possible to reactivate the method at an earlier time by
       *  calling request_trigger(ticks/delay) with an argument that
       *  specifies an earlier clock tick. In order to wake up the method
       *  at a later time, it is first necessary to cancel the pending
       *  trigger with a call to cancel_trigger() */
      void next_trigger(sc_core::sc_time delay) {
	 request_trigger(delay);
	 next_trigger();
      }

      virtual const char* kind() const;
      
    private:
      sc_core::sc_event mEvent;

      virtual void handle_clock_tick() {
	 mEvent.notify();
      }
   };
}

#endif
// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
