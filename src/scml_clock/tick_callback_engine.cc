/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 

#include "tick_callback_engine.h"
#include "clocked_module.h"
#include "scml_clock.h"
#include "scml2/tagged_message_macros.h"
#include <sysc/kernel/sc_spawn.h>
#include <assert.h>

#define RAW_SC_TIME(value) sc_core::sc_time((value), false)

namespace n_scml_clock {
   class TickTriggerHelper {
    private:
      n_scml_clock::tick_callback_engine* mEngine;
    public:
      TickTriggerHelper(n_scml_clock::tick_callback_engine* e) : mEngine(e) {}
      void operator() () { mEngine->handle_tick1_trigger(); }
   };

   class TickTrigger {
    public:
      sc_core::sc_event mEvent;
      TickTrigger* mNext;
    private:
      sc_process_handle mHandle;
    public:
      
      TickTrigger(scml_clock* master, tick_callback_engine* engine)
      : mEvent(sc_core::sc_gen_unique_name("clock_tick_event")
      ), mHandle()
      {
	 sc_core::sc_spawn_options opts;
	 opts.spawn_method();
	 opts.set_sensitivity(&mEvent);
	 opts.dont_initialize();
	 mHandle = sc_core::sc_spawn(n_scml_clock::TickTriggerHelper(engine), sc_core::sc_gen_unique_name("clock_tick_method"), &opts);
	 // coverity[uninit_member] uninitialized by intention: mNext
      }
      virtual ~TickTrigger() {
	 if(sc_core::sc_is_running()) {
	    mHandle.kill();
	 }
      }
   };
   
   struct TimeNode {
      unsigned long long mScheduledTick;
      scml2::clocked_module* mRequests;
      TickTrigger* mTickTrigger;
      TimeNode*	mNextTime;
   };
   TimeNode* gFreeTimeNode = 0;
   inline TimeNode* create_time_node(scml2::clocked_module* request, unsigned long long scheduled_tick) {
      TimeNode* node = gFreeTimeNode;
      if (node != 0) {
	 gFreeTimeNode = node->mNextTime;
      } else {
      	 node = new TimeNode;
      }
      node->mScheduledTick = scheduled_tick;
      node->mRequests = request;
      node->mTickTrigger = 0;
      return node;
   }
   
   inline void free_time_node(TimeNode* node) {
      node->mNextTime = gFreeTimeNode;
      gFreeTimeNode = node;
      
   }

   static int gEngineCount = 0; // the last one has to delete the free list
}

n_scml_clock::tick_callback_engine::tick_callback_engine(
   scml_clock* master, const sc_core::sc_time& period, const sc_core::sc_time& offset, bool running
) : mMaster(master), mRunning(running), mPeriod(period), mOffset(offset),
    mLastTickCount(0), mLastTickTimeValid(false),
    mFreeTickTrigger(0), mRequestsRoot(0), mProcessingTickCount(~0ull)
{
   ++gEngineCount;
}

n_scml_clock::tick_callback_engine::~tick_callback_engine() {
   {
      n_scml_clock::TimeNode* node = mRequestsRoot;
      while (node != 0) {
	 scml2::clocked_module* request = mRequestsRoot->mRequests;
	 while (request != 0) {
	    request->mScheduledTick = ~0ull;
	    request->mEngine = 0;
	    request = request->mNextRequest;
	 }
	 
      	 TickTrigger* t = node->mTickTrigger;
	 if (t != 0) {
	    t->mEvent.cancel();
	    delete t;
	 }
	 n_scml_clock::TimeNode* delete_node =  node;
	 node =  node->mNextTime;
	 delete delete_node;
      }
   }
   
   while (mFreeTickTrigger !=0 ) {
      n_scml_clock::TickTrigger* next = mFreeTickTrigger->mNext;
      delete mFreeTickTrigger;
      mFreeTickTrigger = next;
   }      

   assert(gEngineCount > 0);
   --gEngineCount;
   if (gEngineCount == 0) {
      while (gFreeTimeNode !=0 ) {
	 n_scml_clock::TimeNode* next = gFreeTimeNode->mNextTime;
	 delete gFreeTimeNode;
	 gFreeTimeNode = next;
      }      
   }
}

bool n_scml_clock::tick_callback_engine::check_at_tick() const {
   sc_core::sc_time now = sc_core::sc_time_stamp();
      
   if (mLastTickTimeValid) {
      sc_core::sc_time tmp = now - mLastTickTime;
      if (tmp == sc_core::SC_ZERO_TIME) {
	 return true;
      }
      unsigned long long tick = tmp.value() / mPeriod.value();
      if (tick == 0) {
	 return false;
      }
      // update cache
      mLastTickCount += tick;
      mLastTickTime += RAW_SC_TIME(tick*mPeriod.value());
      return mLastTickTime == now;
   }

   if (!mRunning) {
      return false;
   }

   if (now<mOffset || !sc_core::sc_start_of_simulation_invoked()) {
      return false;
   }

   sc_core::sc_time tmp = now - mOffset;
   return tmp.value() % mPeriod.value() == 0;
}

void n_scml_clock::tick_callback_engine::get_prev_tick_data(unsigned long long& count, sc_core::sc_time& time) const {
   sc_core::sc_time now = sc_core::sc_time_stamp();
      
   if (mLastTickTimeValid) {
      sc_core::sc_time tmp = now - mLastTickTime;
      if (tmp != sc_core::SC_ZERO_TIME) {
	 unsigned long long tick = tmp.value() / mPeriod.value();
	 if (tick > 0) {
	    mLastTickCount += tick;
	    mLastTickTime += RAW_SC_TIME(tick*mPeriod.value());
	 }
      }
   } else {
      if (!mRunning) {
	 count = mLastTickCount;
	 time = now;
	 return;
      }
      
      if (now<mOffset || !sc_core::sc_start_of_simulation_invoked()) {
	 count = mLastTickCount;
	 time = mOffset-mPeriod;
	 return;
      }
      
      sc_core::sc_time tmp = now - mOffset;
      unsigned long long tick = tmp.value() / mPeriod.value();
      mLastTickCount += tick + 1;
      mLastTickTime = mOffset + RAW_SC_TIME(tick*mPeriod.value());
      mLastTickTimeValid = true;
   }

   count = mLastTickCount;
   time = mLastTickTime;
}

void n_scml_clock::tick_callback_engine::get_prev_tick_data(const sc_core::sc_time& delay, unsigned long long& count, sc_core::sc_time& time) const {
   sc_core::sc_time now = sc_core::sc_time_stamp();
   sc_core::sc_time when = now + delay;

   if (mLastTickTimeValid) {
      sc_core::sc_time tmp = when - mLastTickTime;
      unsigned long long tick = tmp.value()/mPeriod.value();
      count = mLastTickCount + tick;
      time = mLastTickTime + RAW_SC_TIME(tick*mPeriod.value());
      return;
   }
   
   if (!mRunning) {
      unsigned long long ticks = delay.value()/mPeriod.value();
      count = mLastTickCount + ticks;
      time = now + RAW_SC_TIME(ticks*mPeriod.value());
      return;
   }
   
   if (when < mOffset) {
      count = mLastTickCount;
      time = mOffset - mPeriod;
      return;
   }

   if (now<mOffset || !sc_core::sc_start_of_simulation_invoked()) {
      sc_core::sc_time tmp = when - mOffset;
      unsigned long long tick = tmp.value() / mPeriod.value();
      count = mLastTickCount + tick + 1;
      time = mOffset + RAW_SC_TIME(tick*mPeriod.value());
      return;
   }
   
   // update last tick time cache
   sc_core::sc_time tmp = now - mOffset;
   unsigned long long tick = tmp.value() / mPeriod.value();
   mLastTickCount += tick + 1;
   mLastTickTime = mOffset + RAW_SC_TIME(tick*mPeriod.value());
   mLastTickTimeValid = true;

   // recall, this time with valid cache
   get_prev_tick_data(delay, count, time);
}

unsigned long long n_scml_clock::tick_callback_engine::get_tick_count() const {
   // could also be implemented with get_prev_tick_data(...), but this explicit one is a bit faster

   sc_core::sc_time now = sc_core::sc_time_stamp();
      
   if (mLastTickTimeValid) {
      sc_core::sc_time tmp = now - mLastTickTime;
      if (tmp != sc_core::SC_ZERO_TIME) {
	 unsigned long long tick = tmp.value() / mPeriod.value();
	 if (tick > 0) {
	    mLastTickCount += tick;
	    mLastTickTime += RAW_SC_TIME(tick*mPeriod.value());
	 }
      }
   } else if (mRunning && now>=mOffset && sc_core::sc_is_running()) {
      sc_core::sc_time tmp = now - mOffset;
      unsigned long long tick = tmp.value() / mPeriod.value();
      mLastTickCount += tick + 1;
      mLastTickTime = mOffset + RAW_SC_TIME(tick*mPeriod.value());
      mLastTickTimeValid = true;
   }
   return mLastTickCount;
}

unsigned long long n_scml_clock::tick_callback_engine::get_tick_count(const sc_core::sc_time& delay) const {
   // could also be implemented with get_prev_tick_data(delay, ...), but this explicit one is a bit faster

   sc_core::sc_time when = sc_core::sc_time_stamp() + delay;

   if (mLastTickTimeValid) {
      sc_core::sc_time tmp = when - mLastTickTime;
      return mLastTickCount + tmp.value()/mPeriod.value();
   }
   
   if (!mRunning) {
      return mLastTickCount + delay.value()/mPeriod.value();
   }
   
   if (when < mOffset) {
      return mLastTickCount;
   }
   
   sc_core::sc_time tmp = when - mOffset;
   return mLastTickCount + tmp.value()/mPeriod.value() + 1;
}

sc_core::sc_time n_scml_clock::tick_callback_engine::get_tick_time(long long clock_ticks_to_skip) const {
   sc_core::sc_time now = sc_core::sc_time_stamp();
      
   if (mLastTickTimeValid) {
      sc_core::sc_time tmp = now - mLastTickTime;
      if (tmp != sc_core::SC_ZERO_TIME) {
	 clock_ticks_to_skip += tmp.value() / mPeriod.value();
      }
      ++clock_ticks_to_skip;
      return mLastTickTime + RAW_SC_TIME(clock_ticks_to_skip*mPeriod.value());
   }

   if (!mRunning) {
      ++clock_ticks_to_skip;
      return now + RAW_SC_TIME(clock_ticks_to_skip*mPeriod.value());
   }

   if (now<mOffset || !sc_core::sc_start_of_simulation_invoked()) {
      return mOffset + RAW_SC_TIME(clock_ticks_to_skip*mPeriod.value());
   }

   sc_core::sc_time tmp = now - mOffset;
   clock_ticks_to_skip += tmp.value( ) /mPeriod.value();
   ++clock_ticks_to_skip;
   return mOffset + RAW_SC_TIME(clock_ticks_to_skip*mPeriod.value());
}

inline n_scml_clock::TickTrigger* n_scml_clock::tick_callback_engine::create_tick_trigger() {
   if (mFreeTickTrigger == 0) {
      return new TickTrigger(mMaster, this);
   }
   TickTrigger* t = mFreeTickTrigger;
   mFreeTickTrigger = t->mNext;
   return t;
}

inline void n_scml_clock::tick_callback_engine::free_tick_trigger(n_scml_clock::TickTrigger* t) {
   t->mNext = mFreeTickTrigger;
   mFreeTickTrigger = t;
}

inline void n_scml_clock::tick_callback_engine::cancel_all_tick_triggers() {
   for (n_scml_clock::TimeNode* node=mRequestsRoot; node!=0; node=node->mNextTime) {
      TickTrigger* t = node->mTickTrigger;
      if (t == 0) {
	 continue;
      }
      t->mEvent.cancel();
      free_tick_trigger(t);
      node->mTickTrigger = 0;
   }
}
   

void n_scml_clock::tick_callback_engine::handle_tick1_trigger() {
   assert(mRunning);
   assert(mRequestsRoot != 0);
   assert(!is_processing_tick());
   assert(mProcessingTickCount == ~0ull);
   
   sc_core::sc_time now = sc_core::sc_time_stamp();
   
   mLastTickCount = mProcessingTickCount = mRequestsRoot->mScheduledTick;
   mLastTickTime = now;
   mLastTickTimeValid = true;
   
   scml2::clocked_module* request = mRequestsRoot->mRequests;
   { // unlink time node
      n_scml_clock::TimeNode* time_node = mRequestsRoot;
      mRequestsRoot = time_node->mNextTime;
      free_tick_trigger(time_node->mTickTrigger);
      n_scml_clock::free_time_node(time_node);
   }
   
   while (request != 0) {
      assert(request->mScheduledTick == mProcessingTickCount);
      scml2::clocked_module* next_request = request->mNextRequest;
      request->mScheduledTick = ~0ull;
      request->handle_clock_tick();
      request = next_request;
   }

   assert(mRunning);
   if (mRequestsRoot!=0 && mRequestsRoot->mTickTrigger==0) {
      mRequestsRoot->mTickTrigger = create_tick_trigger();
      sc_core::sc_time delay = RAW_SC_TIME(mPeriod.value()*(mRequestsRoot->mScheduledTick - mProcessingTickCount));
      mRequestsRoot->mTickTrigger->mEvent.notify(delay);
   }
   mProcessingTickCount = ~0ll;
}

void n_scml_clock::tick_callback_engine::request_trigger(scml2::clocked_module* request, long long clock_ticks_to_skip) {
   if (clock_ticks_to_skip < 0) {
      SCML2_LOG_ERROR(mMaster, std::cerr, SCML_INVALID_API_USAGE) << "Fatal error in SCML clock \"" << mMaster->name() << "\": in method request_trigger():" << std::endl
		<< "             negative clock_ticks_to_skip provided, must be zero or positiv." << std::endl;
      abort();
   }
   
   unsigned long long prev_tick;
   sc_core::sc_time prev_tick_time;

   get_prev_tick_data(prev_tick, prev_tick_time);

   long long delta_ticks = clock_ticks_to_skip + 1;
   
   if (request_trigger_impl(request, prev_tick+delta_ticks)) {
      assert(mRequestsRoot!=0 && mRequestsRoot->mTickTrigger==0);
      mRequestsRoot->mTickTrigger = create_tick_trigger();
      sc_core::sc_time delay = prev_tick_time + RAW_SC_TIME(delta_ticks*mPeriod.value()) - sc_core::sc_time_stamp();
      mRequestsRoot->mTickTrigger->mEvent.notify(delay);
   }
}

void n_scml_clock::tick_callback_engine::request_trigger(scml2::clocked_module* request, const sc_core::sc_time& delay) {
   unsigned long long prev_tick;
   sc_core::sc_time prev_tick_time;

   get_prev_tick_data(delay, prev_tick, prev_tick_time);

   if (!mRunning) {
      SCML2_LOG_ERROR(mMaster, std::cerr, SCML_INVALID_API_USAGE) << "Warning in SCML clock \"" << mMaster->name()
		<< "\": request_trigger() is called on a disabled clock! Could be a violation of the modeling guidelines."
		<< std::endl;
   } else if (mLastTickCount == 0) {
      SCML2_LOG_ERROR(mMaster, std::cerr, SCML_INVALID_API_USAGE) << "Warning in SCML clock \"" << mMaster->name()
		<< "\": request_trigger() is called before the first clock tick! Could be a violation of the modeling guidelines."
		<< std::endl;
   }
   
   if (request_trigger_impl(request, prev_tick+1)) {
      assert(mRequestsRoot!=0 && mRequestsRoot->mTickTrigger==0);
      mRequestsRoot->mTickTrigger = create_tick_trigger();
      sc_core::sc_time delay = prev_tick_time + mPeriod - sc_core::sc_time_stamp();
      mRequestsRoot->mTickTrigger->mEvent.notify(delay);
   }
}

bool n_scml_clock::tick_callback_engine::request_trigger_impl(scml2::clocked_module* request, unsigned long long scheduled_tick) {
   assert(request->mEngine == this);


   if (request->is_clock_trigger_requested()) {
      if (scheduled_tick >= request->mScheduledTick) {
	 return false; // ignore retrigger request for a later tick
      }
      cancel_clock_trigger_impl(request);
   }

   request->mScheduledTick = scheduled_tick;
   
   if (mRequestsRoot == 0) {
      mRequestsRoot = n_scml_clock::create_time_node(request, scheduled_tick);
      mRequestsRoot->mNextTime = 0; // end marker
      request->mNextRequest = 0; // end marker
      return (!is_processing_tick() && mRunning);
   }
   if (scheduled_tick < mRequestsRoot->mScheduledTick) {
      n_scml_clock::TimeNode* time_node = n_scml_clock::create_time_node(request, scheduled_tick);
      time_node->mNextTime = mRequestsRoot; // end marker
      mRequestsRoot = time_node;
      request->mNextRequest = 0; // end marker
      return (!is_processing_tick() && mRunning);
   }
   if (scheduled_tick == mRequestsRoot->mScheduledTick) {
      request->mNextRequest = mRequestsRoot->mRequests;
      mRequestsRoot->mRequests = request;
      return false;
   }

   n_scml_clock::TimeNode* last_time = mRequestsRoot;
   n_scml_clock::TimeNode* cur_time = last_time->mNextTime;
   while (cur_time != 0) {
      if (scheduled_tick < cur_time->mScheduledTick) {
	 n_scml_clock::TimeNode* time_node = n_scml_clock::create_time_node(request, scheduled_tick);
	 time_node->mNextTime = cur_time;
	 last_time->mNextTime = time_node;
	 request->mNextRequest = 0; // end marker
	 return false;
      }
      if (scheduled_tick == cur_time->mScheduledTick) {
	 request->mNextRequest = cur_time->mRequests;
	 cur_time->mRequests = request;
	 return false;
      }
      last_time = cur_time;
      cur_time = last_time->mNextTime;
   }
   {
      n_scml_clock::TimeNode* time_node = n_scml_clock::create_time_node(request, scheduled_tick);
      time_node->mNextTime = 0; // end marker
      last_time->mNextTime = time_node;
      request->mNextRequest = 0; // end marker
   }
   return false;
}

void n_scml_clock::tick_callback_engine::cancel_clock_trigger(scml2::clocked_module* request) {
   if (!request->is_clock_trigger_requested()) {
      return;
   }
   if (request->mScheduledTick == mProcessingTickCount) {
      SCML2_LOG_ERROR(mMaster, std::cerr, SCML_INVALID_API_USAGE) << "Fatal error in SCML clock \"" << mMaster->name() << "\": in method cancel_clock_trigger() while processing a clock tick:" << std::endl
		<< "             Canceling a request that is scheduled for the current tick it is not permitted!" << std::endl
		<< "             Aborting simulation!" << std::endl;
      abort();
   }
   
   cancel_clock_trigger_impl(request);
}

void n_scml_clock::tick_callback_engine::cancel_clock_trigger_impl(scml2::clocked_module* request) {
   assert(mRequestsRoot != 0);
   if (mRequestsRoot->mRequests == request) {
      if (request->mNextRequest == 0) {
	 n_scml_clock::TimeNode* time_node = mRequestsRoot;
      	 mRequestsRoot = time_node->mNextTime;
	 if (time_node->mTickTrigger != 0) {
	    time_node->mTickTrigger->mEvent.cancel();
	    free_tick_trigger(time_node->mTickTrigger);
	    n_scml_clock::free_time_node(time_node);
	 }
	 if (!is_processing_tick() && mRunning && mRequestsRoot!=0 && mRequestsRoot->mTickTrigger==0) {
	    unsigned long long tick_count;
	    sc_core::sc_time prev_tick_time;
	    
	    get_prev_tick_data(tick_count, prev_tick_time);
	    
	    sc_core::sc_time delay =
	       prev_tick_time-sc_core::sc_time_stamp()
	       + RAW_SC_TIME(mPeriod.value()*(mRequestsRoot->mScheduledTick - tick_count));

	    mRequestsRoot->mTickTrigger = create_tick_trigger();
	    mRequestsRoot->mTickTrigger->mEvent.notify(delay);
	 }
      } else {
	 mRequestsRoot->mRequests = request->mNextRequest;
      }
      request->mScheduledTick = ~0ull;
      return;
   }

   n_scml_clock::TimeNode* last_top_time = 0;
   n_scml_clock::TimeNode* cur_time = mRequestsRoot;
   while (true) {
      assert(cur_time != 0);
      if (cur_time->mRequests == request) {
	 if (request->mNextRequest == 0) {
	    (last_top_time==0? mRequestsRoot : last_top_time->mNextTime) = cur_time->mNextTime;
	    if (cur_time->mTickTrigger != 0) {
	       cur_time->mTickTrigger->mEvent.cancel();
	       free_tick_trigger(cur_time->mTickTrigger);
	    }
	    n_scml_clock::free_time_node(cur_time);
	 } else {
	    cur_time->mRequests = request->mNextRequest;
	 }
	 request->mScheduledTick = ~0ull;
	 return;
      }
      scml2::clocked_module* last_request = cur_time->mRequests;
      scml2::clocked_module* cur_request;
      while ((cur_request=last_request->mNextRequest) != 0) {
	 if (cur_request == request) {
	    last_request->mNextRequest = request->mNextRequest;
	    request->mScheduledTick = ~0ull;
	    return;
	 }
	 last_request = cur_request;
      }
      last_top_time = cur_time;
      cur_time = last_top_time->mNextTime;
   }
}

void n_scml_clock::tick_callback_engine::update_parameters(const sc_core::sc_time& next_period, const sc_core::sc_time& next_offset, bool next_running) {
   sc_core::sc_time now = sc_core::sc_time_stamp();

   if (!next_running) { // disabling
      if (mRunning) {
	 mLastTickCount = get_tick_count();
	 mLastTickTimeValid = false;
	 
	 cancel_all_tick_triggers();
	 mRunning = false;
      }
      mPeriod = next_period;
      mOffset = next_offset;
      return;
   }

   unsigned long long last_tick_count = get_tick_count();

   sc_core::sc_time last_period = mPeriod;
   sc_core::sc_time last_offset = mOffset;
   bool last_running = mRunning;
   
   mPeriod = next_period;
   mOffset = next_offset;
   mRunning = next_running;

   if (last_running) { // was already running, only changing period
      if (last_period==mPeriod && last_offset==mOffset) {
	 return; // no change
      }
      mLastTickCount = last_tick_count;
      cancel_all_tick_triggers();
   }
      
   mLastTickTimeValid = false;
   
   // reschedule event
   if (mRequestsRoot != 0) {
      assert(mRequestsRoot->mTickTrigger == 0);
      mRequestsRoot->mTickTrigger = create_tick_trigger();

      unsigned long long scheduled_tick_count = mRequestsRoot->mScheduledTick;
      assert(scheduled_tick_count > mLastTickCount);
      sc_core::sc_time delay = mOffset + RAW_SC_TIME(mPeriod.value()*(scheduled_tick_count-mLastTickCount-1)) - now;
      mRequestsRoot->mTickTrigger->mEvent.notify(delay);
   }
}

// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
