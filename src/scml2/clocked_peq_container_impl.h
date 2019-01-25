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
#ifndef SCML2_CLOCKED_PEQ_CONTAINER_IMPL_H
#define SCML2_CLOCKED_PEQ_CONTAINER_IMPL_H

#include <scml_clock/scml_clock.h>
#include <scml2_logging/snps_vp_dll.h>
#include <assert.h>

namespace scml2 {

   class clocked_peq_container_const_iterator_impl;
   class clocked_peq_container_iterator_impl;
   
   /** This is an internal class that should not be used directly by the
    *  user. */
   class SNPS_VP_API clocked_peq_container_impl {
    public:
      struct entry {
	 entry(void* payload, unsigned long long arrival_tick, entry* next) : mPayload(payload), mArrivalTick(arrival_tick), mNext(next) {}
	 void* mPayload;
	 unsigned long long mArrivalTick;
	 entry* mNext;
      };
      static entry* new_entry(void* payload, unsigned long long arrival_tick, entry* next);
      static void delete_entry(entry*);
      
      clocked_peq_container_impl();
      ~clocked_peq_container_impl();
      
      void set_clock(scml_clock_if* clock) { mClock=clock; }
      scml_clock_if* get_clock() const { return mClock; }
      
      bool notify(void* payload, const sc_core::sc_time& arrival_local_time);
      bool notify(void* payload, const sc_core::sc_time& arrival_local_time, long long delay_ticks);
      bool notify(void* payload, unsigned long long clock_ticks_to_skip);
      bool notifyAt(void* payload, unsigned long long arrival_tick_count);

      bool is_empty() const;

      clocked_peq_container_const_iterator_impl begin() const;
      clocked_peq_container_iterator_impl begin();
      clocked_peq_container_const_iterator_impl end() const;
      clocked_peq_container_iterator_impl end();

      void validateVisible(const entry* &pos) const;
      void validateVisible(entry* &pos) const;
      void increment(const entry* &pos) const;
      void increment(entry* &pos) const;

      void* get_next();
      bool remove(void* payload);

      bool has_more_events() const;
      unsigned long long get_next_event_arrival_tick() const;

    protected:
      scml_clock_if* mClock;
      entry* mFirstEntry;
      static int gInstanceCount;
      static entry* gFreeList;
   };

   class SNPS_VP_API clocked_peq_container_iterator_impl {
    public:
      typedef clocked_peq_container_iterator_impl self;
      typedef clocked_peq_container_impl owner;

      clocked_peq_container_iterator_impl();
      clocked_peq_container_iterator_impl(owner* peq, owner::entry* pos, owner::entry** last_ptr);

      void* get_payload() const;
      void increment();
      bool operator==(const self& x) const;
      bool operator!=(const self& x) const;
      void remove_payload_from_peq() const;
      
    private:
      owner* mPEQ;
      owner::entry* mPos;
      owner::entry** mLastPtr;

      friend class clocked_peq_container_const_iterator_impl;
   };

   class SNPS_VP_API clocked_peq_container_const_iterator_impl {
    public:
      typedef clocked_peq_container_const_iterator_impl self;
      typedef clocked_peq_container_impl owner;

      clocked_peq_container_const_iterator_impl();
      clocked_peq_container_const_iterator_impl(const owner* peq, const owner::entry* pos);
      clocked_peq_container_const_iterator_impl(const clocked_peq_container_iterator_impl& x);

      const void* get_payload() const;
      void increment();
      bool operator==(const self& x) const;
      bool operator!=(const self& x) const;

    private:
      const owner* mPEQ;
      const owner::entry* mPos;
   };

}

inline scml2::clocked_peq_container_impl::clocked_peq_container_impl()
: mClock(0), mFirstEntry(0)
{
   ++gInstanceCount;
}

inline bool scml2::clocked_peq_container_impl::notify(void* payload, const sc_core::sc_time& arrival_local_time) {
   return notifyAt(payload, mClock->get_tick_count(arrival_local_time));
}

inline bool scml2::clocked_peq_container_impl::notify(void* payload, const sc_core::sc_time& arrival_local_time, long long delay_ticks) {
   return notifyAt(payload, mClock->get_tick_count(arrival_local_time)+delay_ticks);
}

inline bool scml2::clocked_peq_container_impl::notify(void* payload, unsigned long long clock_ticks_to_skip) {
   return notifyAt(payload, mClock->get_tick_count()+clock_ticks_to_skip);
}

inline bool scml2::clocked_peq_container_impl::is_empty() const {
   return mFirstEntry==0 || mFirstEntry->mArrivalTick>=mClock->get_tick_count();
}

inline scml2::clocked_peq_container_const_iterator_impl scml2::clocked_peq_container_impl::end() const {
   return clocked_peq_container_const_iterator_impl(this, 0);
}
inline scml2::clocked_peq_container_iterator_impl scml2::clocked_peq_container_impl::end() {
   return clocked_peq_container_iterator_impl(this, 0, 0);
}

inline void scml2::clocked_peq_container_impl::validateVisible(const entry* &pos) const {
   if (pos!=0 && pos->mArrivalTick>=mClock->get_tick_count()) {
      pos = 0;
   }
}

inline void scml2::clocked_peq_container_impl::validateVisible(entry* &pos) const {
   if (pos!=0 && pos->mArrivalTick>=mClock->get_tick_count()) {
      pos = 0;
   }
}

inline void scml2::clocked_peq_container_impl::increment(const entry* &pos) const {
   pos=pos->mNext;
   validateVisible(pos);
}

inline void scml2::clocked_peq_container_impl::increment(entry* &pos) const {
   pos=pos->mNext;
   validateVisible(pos);
}

inline bool scml2::clocked_peq_container_impl::has_more_events() const {
   return mFirstEntry!=0;
}

inline unsigned long long scml2::clocked_peq_container_impl::get_next_event_arrival_tick() const {
   return mFirstEntry->mArrivalTick;
}

inline scml2::clocked_peq_container_iterator_impl::clocked_peq_container_iterator_impl()
: mPEQ(0), mLastPtr(0)
{}

inline scml2::clocked_peq_container_iterator_impl::clocked_peq_container_iterator_impl(
   owner* peq, owner::entry* pos, owner::entry** last_ptr
) : mPEQ(peq), mPos(pos), mLastPtr(last_ptr)
{}

inline void* scml2::clocked_peq_container_iterator_impl::get_payload() const {
   return mPos->mPayload;
}

inline void scml2::clocked_peq_container_iterator_impl::increment() {
   mLastPtr = &mPos->mNext;
   mPEQ->increment(mPos);
}

inline bool scml2::clocked_peq_container_iterator_impl::operator==(const self& x) const {
   return mPos==x.mPos;
}

inline bool scml2::clocked_peq_container_iterator_impl::operator!=(const self& x) const {
   return mPos!=x.mPos;
}

inline scml2::clocked_peq_container_const_iterator_impl::clocked_peq_container_const_iterator_impl()
: mPEQ(0), mPos()
{}

inline scml2::clocked_peq_container_const_iterator_impl::clocked_peq_container_const_iterator_impl(
   const owner* peq, const owner::entry* pos
) : mPEQ(peq), mPos(pos)
{}

inline scml2::clocked_peq_container_const_iterator_impl::clocked_peq_container_const_iterator_impl(
   const clocked_peq_container_iterator_impl& x
) : mPEQ(x.mPEQ), mPos(x.mPos)
{}

inline const void* scml2::clocked_peq_container_const_iterator_impl::get_payload() const {
   return mPos->mPayload;
}

inline void scml2::clocked_peq_container_const_iterator_impl::increment() {
   mPEQ->increment(mPos);
}

inline bool scml2::clocked_peq_container_const_iterator_impl::operator==(const self& x) const {
   return mPos==x.mPos;
}
inline bool scml2::clocked_peq_container_const_iterator_impl::operator!=(const self& x) const {
   return mPos!=x.mPos;
}

#endif
// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
