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
#ifndef SCML_CLOCK_TICK_CALLBACK_ENGINE_H
#define SCML_CLOCK_TICK_CALLBACK_ENGINE_H

#include <systemc>
#include <scml2_logging/snps_vp_dll.h>

namespace scml2 {
   class clocked_module;
}

class scml_clock;

namespace n_scml_clock {

   class TickTriggerHelper;
   class TickTrigger;
   struct TimeNode;
   
   class SNPS_VP_API tick_callback_engine {
    public:
      scml_clock* get_master() const { return mMaster; }
      bool check_at_tick() const;
      unsigned long long get_tick_count() const;
      unsigned long long get_tick_count(const sc_core::sc_time& delay) const;
      sc_core::sc_time get_tick_time(long long clock_ticks_to_skip) const;
      void get_next_tick_data(const sc_core::sc_time& delay, unsigned long long& count, sc_core::sc_time& time) const;

    private:
      tick_callback_engine(scml_clock* master, const sc_core::sc_time& period, const sc_core::sc_time& offset, bool running);
      ~tick_callback_engine();
      
      void update_parameters(const sc_core::sc_time& period, const sc_core::sc_time& offset, bool running);
      
      scml_clock* mMaster;
      bool mRunning;
      sc_core::sc_time mPeriod;
      sc_core::sc_time mOffset;
      
      void get_prev_tick_data(unsigned long long& count, sc_core::sc_time& time) const;
      void get_prev_tick_data(const sc_core::sc_time& delay, unsigned long long& count, sc_core::sc_time& time) const;
      void request_trigger(scml2::clocked_module* request, long long clock_ticks_to_skip);
      void request_trigger(scml2::clocked_module* request, const sc_core::sc_time& delay);
      bool request_trigger_impl(scml2::clocked_module* request, unsigned long long scheduled_tick);
      void cancel_clock_trigger(scml2::clocked_module*);
      void cancel_clock_trigger_impl(scml2::clocked_module*);
      void handle_tick1_trigger();
      
      mutable unsigned long long mLastTickCount;
      mutable bool mLastTickTimeValid;
      mutable sc_core::sc_time mLastTickTime;

      TickTrigger* mFreeTickTrigger;
      TickTrigger* create_tick_trigger();
      void free_tick_trigger(TickTrigger*);
      void cancel_all_tick_triggers();
      
      TimeNode* mRequestsRoot;
      unsigned long long mProcessingTickCount; // ~0ull: not processing tick
      bool is_processing_tick() const { return mProcessingTickCount!=~0ull; }

      friend class ::scml_clock;
      friend class scml2::clocked_module;
      friend class n_scml_clock::TickTriggerHelper;

      // Prohibit usage of automatically generated member functions
      tick_callback_engine(const tick_callback_engine&);
      tick_callback_engine& operator =(const tick_callback_engine&);
   };
   
}

inline void n_scml_clock::tick_callback_engine::get_next_tick_data(
   const sc_core::sc_time& delay, unsigned long long& count, sc_core::sc_time& time
) const {
   get_prev_tick_data(delay, count, time);
   ++count;
   time += mPeriod;
}

#endif
// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
