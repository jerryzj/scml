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
#include "clocked_peq_container_impl.h"

int scml2::clocked_peq_container_impl::gInstanceCount = 0;
scml2::clocked_peq_container_impl::entry* scml2::clocked_peq_container_impl::gFreeList = 0;

scml2::clocked_peq_container_impl::entry* scml2::clocked_peq_container_impl::new_entry(
   void* payload, unsigned long long arrival_tick, entry* next
) {
   entry* e = gFreeList;
   if (e == 0) {
      return new entry(payload, arrival_tick, next);
   }
   gFreeList = e->mNext;
   e->mPayload = payload;
   e->mArrivalTick = arrival_tick;
   e->mNext = next;
   return e;
}

void scml2::clocked_peq_container_impl::delete_entry(entry* e) {
   e->mNext = gFreeList;
   gFreeList = e;
}

scml2::clocked_peq_container_impl::~clocked_peq_container_impl() {
   --gInstanceCount;
   if (gInstanceCount == 0) {
      while (gFreeList != 0) {
	 entry* e = gFreeList;
	 gFreeList = e->mNext;
	 delete e;
      }
   }
}

bool scml2::clocked_peq_container_impl::notifyAt(void* payload, unsigned long long arrival_tick_count) {
   entry* pos = mFirstEntry;
   entry** last_ptr = &mFirstEntry;
   while (pos != 0) {
      if (pos->mArrivalTick > arrival_tick_count) {
	 break;
      }
      last_ptr = &pos->mNext;
      pos = *last_ptr;
   }

   *last_ptr = new_entry(payload, arrival_tick_count, pos);
   return last_ptr==&mFirstEntry;
}

scml2::clocked_peq_container_const_iterator_impl scml2::clocked_peq_container_impl::begin() const {
   const entry* pos = mFirstEntry;
   validateVisible(pos);
   return clocked_peq_container_const_iterator_impl(this, pos);
}

scml2::clocked_peq_container_iterator_impl scml2::clocked_peq_container_impl::begin() {
   entry* pos = mFirstEntry;
   validateVisible(pos);
   return clocked_peq_container_iterator_impl(this, pos, &mFirstEntry);
}

void* scml2::clocked_peq_container_impl::get_next() {
   entry* pos = mFirstEntry;
   validateVisible(pos);
   if (pos == 0) {
      return 0;
   }
   void* payload = pos->mPayload;
   mFirstEntry = pos->mNext;
   delete_entry(pos);
   return payload;
}

bool scml2::clocked_peq_container_impl::remove(void* payload) {
   unsigned long long now = mClock->get_tick_count();
   entry** last_ptr = &mFirstEntry;
   entry* pos = mFirstEntry;
   while (pos != 0) {
      if (pos->mArrivalTick >= now) {
	 return false;
      }
      if (pos->mPayload == payload) {
	 *last_ptr = pos->mNext;
	 delete_entry(pos);
	 return true;
      }
      last_ptr = &pos->mNext;
      pos = *last_ptr;
   }
   return false;
}

void scml2::clocked_peq_container_iterator_impl::remove_payload_from_peq() const {
   *mLastPtr = mPos->mNext;
   clocked_peq_container_impl::delete_entry(mPos);
}


// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
