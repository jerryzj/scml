/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_CLOCKED_TIMER_H
#define SCML2_CLOCKED_TIMER_H

#include "clocked_callback.h"
#include <scml2/resource.h>
#include <scml2_logging/snps_vp_dll.h>

namespace scml2 {
   

   /** A clocked timer is a modeling object that provides an alarm callback
    * mechanism based on an SCML clock. The timer can be started to expire
    * after a given number of clock ticks. When it expires, it calls a
    * callback. The timer can be configured to expire once, multiple times,
    * or to run forever. It is possible to stop a running timer and to
    * resume it at a later time. */
   class SNPS_VP_API clocked_timer : public sc_core::sc_object, public clocked_module {
    public:
      clocked_timer(const std::string& name, scml_clock_if* clock=0);
      virtual ~clocked_timer();
      
      /** Associates the clocked_timer with the provided SCML2 clock
       *  CLOCK. */
      void set_clock(scml_clock_if* clock) {
	 clocked_module::set_clock(clock);
      }
      /** Associates the clocked_timer with the SCML clock, which is bound
       *  to the provided port P. */
      void set_clock(sc_core::sc_in<bool>& p) {
	 clocked_module::set_clock(p);
      }
      /** Returns the associated SCML2 clock. */
      scml_clock_if* get_clock() const {
	 return clocked_module::get_clock();
      }

      /** Register the member function FUNC of class MOD as callback for
       *  alarm notifications. */
      template <class MOD_TYPE, typename FUNCPTR_TYPE>
      void set_callback(MOD_TYPE* mod, FUNCPTR_TYPE func) {
        set_callback(new clocked_method_callback<MOD_TYPE, FUNCPTR_TYPE>(mod, func));
      }

      void set_callback(clocked_method_callback_base* callback);

      /** Start the timer. It will expire after TICKS_PER_PERIOD ticks. For
       *  example a timer started with start(1) will expire at the next
       *  clock tick. With TICKS_PER_PERIOD=0 the timer does not start.
       *
       *  The argument SHOT_COUNT specifies how many alarm callbacks will
       *  be triggered by the timer with a distance of TICKS_PER_PERIOD
       *  ticks. With SHOT_COUNT=1 the timer expires once. With
       *  SHOT_COUNT=-1 (default) the timer runs forever or until it is
       *  stopped by calling stop(). With SHOT_COUNT=0 the timer does not
       *  start.
       *
       *  If start() is called while the timer running, it will first be
       *  stopped before it is restarted. If a running timer is started()
       *  with TICKS_PER_PERIOD=0 or SHOT_COUNT=0, the timer is stopped and
       *  left in the idle state, so that it is not possible to resume it
       *  at a later time. */
      void start(long long ticks_per_period, long long shot_count=-1);
      /** Start the timer. This is a convenience method comparable to
       *  start(long long, long long). The period in clock ticks is derived
       *  by the sc_time argument PERIOD by rounding up to the next higher
       *  multiple of the current clock period. For more details see the
       *  description of the method start(long long, long long). */
      void start(const sc_core::sc_time& period, long long shot_count=-1);
      /** Stop a running timer. The timer is left in a state that allows it
       *  to be resumed at a later time. It has no effect if called on an
       *  idle or already stopped timer. */
      void stop();
      /** Resume a stopped timer. Only timers that have been stopped by
       *  stop() can be resumed. In that case the timer continues to run
       *  with the status at which it was stopped. If the timer was stopped
       *  in the middle of a period, than the timer finishes the already
       *  started period. For example a timer started at clock tick 100
       *  with period=10 and shot_count=3, stopped at time 115 (after one
       *  alarm and another half period), and resumed at time 200, will
       *  expire at the times 205 and 215.
       *
       *  It returns true on success. False is returned if it is called on
       *  an idle or already running timer. */
      bool resume_stopped();
      /** Return the value of the internal counter of the timer. It counts the
       *  number of clock ticks since the last start or restart. */
      unsigned long long get_counter_value();
      enum eState { eS_Idle=0, eS_Running, eS_Stopped };
      static const char* const gStateString[];

      enum eEvent { eE_Start=0, eE_Expired, eE_Stop, eE_ResumeStopped, eE_Delete };
      static const char* const gEventString[];
      
      /** Returns the state of the timer. In order to avoid dependency on
       *  scheduling order, the state at the beginning of the current
       *  SystemC time is returned. A change to the state will only become
       *  visible after the SystemC time advanced. If called from the timer
       *  callback, than the state from before the current clock cycle is
       *  returned. */
      eState get_state() { update_cache(); return mCachedState; }
      /** Returns true of the timer is running. In order to avoid
       *  dependency on scheduling order, the activity state at the
       *  beginning of the current SystemC time is returned. A change to
       *  the state will only become visible after the SystemC time
       *  advanced. If called from the timer callback, than the state from
       *  before the current clock cycle is returned.*/
      bool is_active() { update_cache(); return mCachedState==eS_Running; }
      /** Returns the number of remaining shots. In order to avoid
       *  dependency on scheduling order, the activity state at the
       *  beginning of the current SystemC time is returned. A change to
       *  the state will only become visible after the SystemC time
       *  advanced. If called from the timer callback, than the value from
       *  before the current clock cycle is returned. */
      long long get_remaining_shot_count() { update_cache(); return mCachedRemainingShotCount; }
      
      virtual const char* kind() const;
      sc_core::sc_module* get_owner() const { return mOwner; }
      

      long long get_next_ticks_per_period() const { return mTicksPerPeriod; }
    private:
      clocked_method_callback_base* mCallback;

      eState mState;
      bool mCallbackPending;
      
      long long mTicksPerPeriod;
      long long mRemainingShotCount;
      long long mRemainingTicksWhenResuming;

      void update_cache(bool from_callback=false);

      sc_core::sc_time mCacheTimeStamp;
      eState mCachedState;
      long long mCachedRemainingShotCount;
      
      sc_core::sc_module* mOwner;
      
      virtual void handle_clock_tick();
      
   };


}


#endif
