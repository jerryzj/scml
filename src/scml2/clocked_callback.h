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
#ifndef SCML2_CLOCKED_CALLBACK_H
#define SCML2_CLOCKED_CALLBACK_H

#include "clocked_module.h"
#include <scml2/resource.h>
#include <scml2_logging/snps_vp_dll.h>

#define SCML2_CLOCKED_CALLBACK(funcname) this, &SC_CURRENT_USER_MODULE::funcname

namespace scml2 {
   
   class clocked_method_callback_base : public resource {
    public:
      virtual void notify() = 0;
   };
   
   template<class MOD_TYPE, typename FUNCPTR_TYPE>
   class clocked_method_callback : public clocked_method_callback_base {
    public:
      clocked_method_callback(MOD_TYPE* mod, FUNCPTR_TYPE func) : mMod(mod), mFunc(func) {}
      virtual void notify() { (mMod->*mFunc)(); }
    private:
      MOD_TYPE * mMod;
      FUNCPTR_TYPE mFunc;
   };
   
   /** A convenience class that forwards a clock tick callback to any
    *  member function of a class without the need to inherit a class from
    *  clocked_module.
    *
    *  A callback received via a clocked_callback involves an internal
    *  method redirection, and is therefore slightly slower than a callback
    *  received by directly inheriting from clocked_module and implementing
    *  handle_clock_trigger(). */
   class SNPS_VP_API clocked_callback : public sc_core::sc_object, public clocked_module {
    public:
      /** Construct a clocked_callback of name NAME that is associated with
       *  the SCML2 clock object CLOCK. */ 
      clocked_callback(const std::string& name, scml_clock_if* clock=0)
      : sc_core::sc_object(name.c_str()), clocked_module(clock), mCallback(0)
      {}
      /** Destructs the clocked_callback. If there is still a pending clock
       *  tick callback request during destruction, it will automatically
       *  be canceled. */
      virtual ~clocked_callback();
      
      /** Associates the clocked_callback with the provided SCML2 clock
       *  CLOCK. */
      void set_clock(scml_clock_if* clock) {
	 clocked_module::set_clock(clock);
      }
      /** Associates the clocked_callback with the SCML clock, which is
       *  bound to the provided port P. */
      void set_clock(sc_core::sc_in<bool>& p) {
	 clocked_module::set_clock(p);
      }
      /** Returns the associated SCML2 clock. */
      scml_clock_if* get_clock() const {
	 return clocked_module::get_clock();
      }

      /** Register the member function FUNC of class MOD as callback for
       *  clock tick. */
      template <class MOD_TYPE, typename FUNCPTR_TYPE>
      void set_callback(MOD_TYPE* mod, FUNCPTR_TYPE func) {
	 set_callback(new clocked_method_callback<MOD_TYPE, FUNCPTR_TYPE>(mod, func));
      }

      void set_callback(clocked_method_callback_base* callback);
      
      /** Schedules a callback for a future clock tick. The argument
       *  CLOCK_TICKS_TO_SKIP defines the number of ticks that shall be
       *  skipped from now. Can be called at any SystemC time.
       *
       *  If called while a previous callback request is still pending,
       *  then the new request is ignored if it targets the same or a later
       *  clock tick. If it targets an earlier clock tick, then the
       *  previous request is canceled, and the request is scheduled for
       *  the new earlier tick.
       *
       *  If a callback was requested, then the requested/scheduled
       *  clock tick can be retrieved by a call to method
       *  get_scheduled_clock_tick().
       *
       *  For example, after a call of request_trigger(0), the clock
       *  tick count returned by get_scheduled_clock_tick() will be
       *  one larger than the current clock tick count of the
       *  associated SCML clock.*/
      void request_trigger(long long clock_ticks_to_skip) {
	 request_clock_trigger(clock_ticks_to_skip);
      }
      /** Schedules a callback for a future clock tick after the time
       *  defined by the argument DELAY. The argument DELAY is a SystemC
       *  time that is interpreted relative to the current SystemC time
       *  sc_time_stamp(). This method will usually be used by TLM2 models
       *  with temporal decoupling in order to synchronize to the next
       *  clock tick after the given local time argument.
       *
       *  If called while a previous callback request is still pending,
       *  then the new request is ignored if it targets the same or a later
       *  clock tick. If it targets an earlier clock tick, then the
       *  previous request is canceled, and the request is scheduled for the
       *  new earlier tick. */
      void request_trigger(const sc_core::sc_time& delay) {
	 request_clock_trigger(delay);
      }
      /** Returns true if a callback was requested and is still pending. */
      bool is_trigger_requested() const {
	 return is_clock_trigger_requested();
      }
      /** Cancels a pending callback request. Does nothing if no callback
       *  request is currently pending. During the processing of a clock
       *  tick (i.e. from a callback, or a handle_clock_tick() call) it is
       *  not permitted to cancel a callback request that was scheduled for
       *  the currently processed clock tick. */
      void cancel_trigger() {
	 cancel_clock_trigger();
      }

      virtual const char* kind() const;
      
    private:
      clocked_method_callback_base* mCallback;

      virtual void handle_clock_tick() {
	 if (mCallback != 0) {
	    mCallback->notify();
	 }
      }
   };


   template<class MOD_TYPE, typename FUNCPTR_TYPE>
   class temp_clocked_method_callback : public clocked_method_callback_base {
    public:
      temp_clocked_method_callback(scml_clock_if* clock, long long clock_ticks_to_skip, MOD_TYPE* mod, FUNCPTR_TYPE func)
      : mMod(mod), mFunc(func)
      {
	 init(clock);
      	 mImpl->request_trigger(clock_ticks_to_skip);
      }
      temp_clocked_method_callback(scml_clock_if* clock, const sc_core::sc_time& delay, MOD_TYPE* mod, FUNCPTR_TYPE func)
      : mMod(mod), mFunc(func)
      {
	 init(clock);
      	 mImpl->request_trigger(delay);
      }
      virtual void notify() {
	 (mMod->*mFunc)();
	 delete mImpl;
      }
    private:
      clocked_callback* mImpl;
      MOD_TYPE * mMod;
      FUNCPTR_TYPE mFunc;

      void init(scml_clock_if* clock) {
	 mImpl = new clocked_callback(sc_core::sc_gen_unique_name("<tmp_clocked_callback>"), clock);
	 mImpl->set_callback(this);
      }
   };
   
   template <class MOD_TYPE, typename FUNCPTR_TYPE>
   void request_clocked_method_callback(scml_clock_if* clock, long long clock_ticks_to_skip, MOD_TYPE* mod, FUNCPTR_TYPE func) {
      (void)new temp_clocked_method_callback<MOD_TYPE, FUNCPTR_TYPE>(clock, clock_ticks_to_skip, mod, func);
   }
   template <class MOD_TYPE, typename FUNCPTR_TYPE>
   void request_clocked_method_callback(sc_core::sc_in<bool>& clock_port, long long clock_ticks_to_skip, MOD_TYPE* mod, FUNCPTR_TYPE func) {
      (void)new temp_clocked_method_callback<MOD_TYPE, FUNCPTR_TYPE>(scml2::get_scml_clock(clock_port), clock_ticks_to_skip, mod, func);
   }

   template <class MOD_TYPE, typename FUNCPTR_TYPE>
   void request_clocked_method_callback(scml_clock_if* clock, const sc_core::sc_time& delay, MOD_TYPE* mod, FUNCPTR_TYPE func) {
      (void)new temp_clocked_method_callback<MOD_TYPE, FUNCPTR_TYPE>(clock, delay, mod, func);
   }
   template <class MOD_TYPE, typename FUNCPTR_TYPE>
   void request_clocked_method_callback(sc_core::sc_in<bool>& clock_port, const sc_core::sc_time& delay, MOD_TYPE* mod, FUNCPTR_TYPE func) {
      (void)new temp_clocked_method_callback<MOD_TYPE, FUNCPTR_TYPE>(scml2::get_scml_clock(clock_port), delay, mod, func);
   }

}

#endif
// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
