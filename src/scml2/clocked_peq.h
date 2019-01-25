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
#ifndef SCML2_CLOCKED_PEQ_H
#define SCML2_CLOCKED_PEQ_H

#include "clocked_peq_container_impl.h"
#include "clocked_callback.h"

namespace scml2 {
   
   template <typename PAYLOAD> class clocked_peq_iterator;
   template <typename PAYLOAD> class clocked_peq_const_iterator;
   
   /** A clocked payload event queue (PEQ) is a convenience modeling object
    *  for TLM2 models with temporal decoupling. It can be used to buffer
    *  input payload that was received via a temporal decoupled transport,
    *  and get a callback when the SystemC advanced to a point when a
    *  payload becomes visible for non-temporal decoupled modules. Each
    *  payload that is pushed into the PEQ is tagged by a timestamp that
    *  corresponds to the local time of the arrival time. The time stamp is
    *  first converted to the tick count of the associated clock, before it
    *  is saved in the PEQ.
    *
    *  The PEQ sorts the payload by its timestamp. The queue reflects the
    *  view of a user that lives in the current SystemC time. It only shows
    *  payload that arrived in the past. That means that all payload with
    *  timestamp >= get_clock()->get_tick_count() is hidden from the user
    *  interface. For payload that arrived in the past, the container
    *  behaves like a single-linked list that is sorted by the timestamp.
    *
    *  The PEQ provides a callback mechanism based on a clocked_module that
    *  allows to be called back at the clock cycle when a hidden payload
    *  becomes visible. It is possible to block the callback of the PEQ. In
    *  that case the callback is delayed until the PEQ is unblocked again.
    *  
    *  Because the timestamp for payload is based on clock tick count and
    *  not SystemC time, the arrival time of all future payload is always
    *  adapted in case of parameter changes of the clock like
    *  period/enable/disable.
    *
    *  The container provides iterators for iterating over the queue in
    *  forward direction. The API of the iterators conforms to the
    *  STL. There is a const and a non-const version of the iterator. The
    *  const iterator provides a faster iteration mechanism. The non-const
    *  iterator allows to remove the payload it is pointing to from the
    *  list. */
   template <typename PAYLOAD>
   class clocked_peq : public clocked_module {
    public:
      typedef clocked_peq_iterator<PAYLOAD> iterator;
      typedef clocked_peq_const_iterator<PAYLOAD> const_iterator;

      /** Construct a clocked PEQ that is associated with the SCML2 clock
       *  object CLOCK. */ 
      clocked_peq(scml_clock_if* clock=0) : mImpl(), mCallback(0), mUnblockClockTick(0) { set_clock(clock); }
      /** Construct a clocked PEQ that is associated with the SCML clock,
       *  which is bound to the provided port P. */ 
      clocked_peq(sc_core::sc_in<bool>& p) : mImpl(), mCallback(0), mUnblockClockTick(0) { set_clock(scml2::get_scml_clock(p)); }

      virtual ~clocked_peq() {
	 if (mCallback != 0) {
	    mCallback->unref();
	 }
      }
      
      /** Register the member function FUNC of class MOD as callback for
       *  payload event notifications. */
      template <class MOD_TYPE, typename FUNCPTR_TYPE>
      void set_callback(MOD_TYPE* mod, FUNCPTR_TYPE func) {
	set_callback(new clocked_method_callback<MOD_TYPE, FUNCPTR_TYPE>(mod, func));
      }
      
      void set_callback(clocked_method_callback_base* callback) {
	 if (mCallback != 0) {
	    mCallback->unref();
	 }
	 mCallback = callback;
	 if (mCallback != 0) {
	    mCallback->ref();
	 }
      }
      
      /** Associates the clocked_peq_container with the provided SCML2
       *  clock CLOCK. */
      void set_clock(scml_clock_if* clock) { mImpl.set_clock(clock); clocked_module::set_clock(clock); }
      /** Associates the clocked_peq_container with the SCML clock, which
       *  is bound to the provided port P. */
      void set_clock(sc_core::sc_in<bool>& p) { set_clock(scml2::get_scml_clock(p)); }
      /** Return the associated SCML2 clock. */
      scml_clock_if* get_clock() const { return mImpl.get_clock(); }
      
      /** Push a payload into the PEQ that was received by a temporal
       *  decoupled transport at local time ARRIVAL_LOCAL_TIME. The local
       *  time argument is given as SystemC time. It is converted to the
       *  clock tick count by rounding down to the last clock tick. */
      bool notify(PAYLOAD* payload, const sc_core::sc_time& arrival_local_time) {
      	 bool at_front = mImpl.notify(payload, arrival_local_time);
	 if (at_front) {
	    schedule_next_clock_trigger();
	 }
	 return at_front;
      }
      /** Push a payload into the PEQ that was received by a temporal
       *  decoupled transport. The local time is defined by two arguments,
       *  the SystemC time ARRIVAL_LOCAL_TIME and the additional number of
       *  clock ticks DELAY_TICKS. The internal arrival clock tick count is
       *  calculated by first converting ARRIVAL_LOCAL_TIME to the clock
       *  tick count by rounding down to the last clock tick, and then
       *  incrementing it by DELAY_TICKS. */
      bool notify(PAYLOAD* payload, const sc_core::sc_time& arrival_local_time, long long delay_ticks) {
      	 bool at_front = mImpl.notify(payload, arrival_local_time, delay_ticks);
	 if (at_front) {
	    schedule_next_clock_trigger();
	 }
	 return at_front;
      }
      /** Push a payload into the PEQ that was received by a temporal
       *  decoupled transport at the local time that is CLOCK_TICKS_TO_SKIP
       *  clock ticks in the future. */
      bool notify(void* payload, unsigned long long clock_ticks_to_skip) {
	 bool at_front = mImpl.notify(payload, clock_ticks_to_skip);
	 if (at_front) {
	    schedule_next_clock_trigger();
	 }
	 return at_front;
      }
      /** Push a payload into the PEQ that was received by a temporal
       *  decoupled transport at the local time represented by the clock
       *  tick count ARRIVAL_TICK_COUNT. */
      bool notifyAt(void* payload, unsigned long long arrival_tick_count) {
	 bool at_front = mImpl.notifyAt(payload, arrival_tick_count);
	 if (at_front) {
	    schedule_next_clock_trigger();
	 }
	 return at_front;
      }

      /** Return true if the PEQ is empty. This only considers visible
       *  payload (timestamp < get_clock()->get_tick_count()). */
      bool is_empty() const { return mImpl.is_empty(); }
      /** Return a const iterator that points to the beginning of the
       *  internal sorted list. This only considers visible payload
       *  (timestamp < get_clock()->get_tick_count()). */
      const_iterator begin() const { return const_iterator(mImpl.begin()); }
      /** Return a non-const iterator that points to the beginning of the
       *  internal sorted list. This only considers visible payload
       *  (timestamp < get_clock()->get_tick_count()). */
      iterator begin() { return iterator(mImpl.begin()); }
      /** Return a const iterator that points to the end of the internal
       *  sorted list. This only considers visible payload (timestamp <
       *  get_clock()->get_tick_count()). */
      const_iterator end() const { return const_iterator(mImpl.end()); }
      /** Return a non-const iterator that points to the end of the
       *  internal sorted list. This only considers visible payload
       *  (timestamp < get_clock()->get_tick_count()). */
      iterator end() { return iterator(mImpl.end()); }

      /** Extract the first element from the internal sorted list. It
       *  returns NULL if the list was empty. This only considers visible
       *  payload (timestamp < get_clock()->get_tick_count()). */
      PAYLOAD* get_next() {
	 PAYLOAD* payload = reinterpret_cast<PAYLOAD*>(mImpl.get_next());
	 if (is_empty() && is_clock_trigger_requested()) {
	    cancel_clock_trigger();
	    if (has_more_events()) {
	       schedule_next_clock_trigger();
	    }
	 }
	 return payload;
      }
      
      /** Remove the payload at position which is given by the iterator
       *  argument POS. The payload itself is not deleted.
       *
       *  This operation invalidates all iterator which are currently
       *  pointing into the container. Managing the iterators is the user's
       *  responsibility.*/
      void remove(const iterator& pos) { pos.mImpl.remove_payload_from_peq(); }
      /** Remove the payload given by the PAYLOAD argument. The payload
       *  itself is not deleted. This only considers visible payload
       *  (timestamp < get_clock()->get_tick_count()). It returns true if
       *  the payload was found and removed. False is returned if the
       *  payload was not found. If the same payload is contained multiple
       *  times, than only the first occurrence is removed.
       *
       *  If this operation succeeds, it invalidates all iterator which are
       *  currently pointing into the container. Managing the iterators is
       *  the user's responsibility.*/
      bool remove(PAYLOAD* payload) { return mImpl.remove(payload); }

      /** Return true if more payload is available in the container. In
       *  contrast to the method is_empty() this considers not only the
       *  visible but all payload. This method is usually used if all
       *  visible/past payload has been processed/removed, in order to
       *  learn if more processing has to be done at a future time, even if
       *  no new payload is pushed into the container. In that case an
       *  unblocked PEQ already scheduled another callback notification for
       *  the future. */
      bool has_more_events() const { return mImpl.has_more_events(); }
      /** Return the arrival clock tick of the next payload in the
       *  PEQ. This is the time tick at which an unblocked PEQ will send
       *  the next callback. This method must not be called if
       *  has_more_events() returned false!
       *
       *  Note that this method does not only consider the visible but all
       *  payload. It is usually used when all current processing has been
       *  done, in order to retrieve the time when the next callback will
       *  notify to continue with more processing. */
      unsigned long long get_next_event_arrival_tick() const { return mImpl.get_next_event_arrival_tick(); }
      
      /** Block the sending of callbacks for the next payload arrival
       *  time. */
      void block() {
	 mUnblockClockTick = ~0ull;
	 cancel_clock_trigger();
      }
      
      /** Schedule to unblock the PEQ after a SystemC time interval of
       *  LOCAL_TIME_BEFORE_CLOCK_TICK. If at the time of unblocking the
       *  PEQ contains a payload with an arrival time that lays in the
       *  past, then a callback is sent at the next clock edge following
       *  the unblock time.
       *
       *  For example: Calling unblock(SC_ZERO_TIME) on a blocked PEQ that
       *  contains a visible payload (arrival time in the past) will
       *  schedule a callback for the next clock tick. */
      void unblock(const sc_core::sc_time& local_time_before_clock_tick) {
	 unblockAt(get_clock()->get_tick_count(local_time_before_clock_tick));
      }
      /** Schedule to unblock the PEQ after a SystemC time interval of
       *  LOCAL_TIME_BEFORE_CLOCK_TICK plus DELAY_TICKS clock ticks. For
       *  more details see method unblock(const sc_core::sc_time&). */
      void unblock(const sc_core::sc_time& local_time_before_clock_tick, long long delay_ticks) {
	 unblockAt(get_clock()->get_tick_count(local_time_before_clock_tick) + delay_ticks);
      }
      /** Schedule to unblock the PEQ after CLOCK_TICKS_TO_SKIP clock
       *  ticks. For more details see method unblock(const
       *  sc_core::sc_time&).
       *
       *  For example: Calling unblock(0) on a blocked PEQ that contains a
       *  visible payload (arrival time in the past) will schedule a
       *  callback for the next clock tick. */
      void unblock(unsigned long long clock_ticks_to_skip) {
	 unblockAt(get_clock()->get_tick_count() + clock_ticks_to_skip);
      }
      /** Schedule to unblock the PEQ for the time given by the clock tick
       *  count TICK_COUNT_FOR_UNBLOCK. If TICK_COUNT_FOR_UNBLOCK lays in
       *  the past, then the PEQ is unblocked at the next tick count. For
       *  more details see method unblock(const sc_core::sc_time&).
       *
       *  For example: Calling unblockAt(get_clock()->get_tick_count()) on
       *  a blocked PEQ that contains a visible payload (arrival time in
       *  the past) will schedule a callback for the next clock tick. */
      void unblockAt(unsigned long long tick_count_for_unblock) {
	 mUnblockClockTick = tick_count_for_unblock;
	 if (has_more_events()) {
	    schedule_next_clock_trigger();
	 }
      }

    private:
      clocked_peq_container_impl mImpl;
      clocked_method_callback_base* mCallback;
      unsigned long long mUnblockClockTick;

      void schedule_next_clock_trigger() {
	 assert(has_more_events());
	 if (mUnblockClockTick == ~0ull) {
	    return;
	 }
 	 unsigned long long now = get_clock()->get_tick_count();
	 unsigned long long tick_count = get_next_event_arrival_tick();
	 if (tick_count < mUnblockClockTick) {
	    tick_count = mUnblockClockTick;
	 }
	 request_clock_trigger(tick_count-now);
      }
      
      virtual void handle_clock_tick() {
	 if (mCallback != 0) {
	    mCallback->notify();
	 }
	 unsigned long long now = get_clock()->get_tick_count();
	 if (mUnblockClockTick < now) {
	    mUnblockClockTick = now;
	 }
	 if (has_more_events()) {
	    schedule_next_clock_trigger();
	 }
      }
   };
   
   /** A pointer into a clocked_peq that allows to iterate in forward
    *  direction over the sorted list of the PEQ. This non-const iterator
    *  can be used to remove the payload it is currently pointing to from
    *  the PEQ. Iterating with this non-const version of the iterator is
    *  slightly slower than iterating with the const version
    *  clocked_peq_const_iterator. Iterators are usually generated by the
    *  begin() and end() method of a PEQ. */
   template <typename PAYLOAD>
   class clocked_peq_iterator {
    public:
      typedef clocked_peq_iterator<PAYLOAD> self;
      typedef clocked_peq_container_impl owner;

      /** Construct an iterator that does not point to any payload. */
      clocked_peq_iterator() : mImpl() {}

      /** Return the payload pointed to by the iterator. */
      PAYLOAD* operator*() const { return reinterpret_cast<PAYLOAD*>(mImpl.get_payload()); }
      /** Return the payload pointed to by the iterator. */
      PAYLOAD* operator->() const { return reinterpret_cast<PAYLOAD*>(mImpl.get_payload()); }

      /** Move the pointer forward to the next entry in the PEQ. It returns
       *  an iterator that points to the new position. */
      self& operator++() { mImpl.increment(); return *this; }
      /** Move the pointer forward to the next entry in the PEQ. It returns
       *  an iterator that points to the old position. */
      self operator++(int) { self t=*this; mImpl.increment(); return t; }
      
      /** Compare the iterator to the other iterator X. It returns true if
       *  both iterators are pointing to the same position. */ 
      bool operator==(const self& x) const { return mImpl==x.mImpl; }
      /** Compare the iterator to the other iterator X. It returns true if
       *  both iterators are pointing to different positions. */ 
      bool operator!=(const self& x) const { return mImpl!=x.mImpl; }

    private:
      clocked_peq_iterator(const clocked_peq_container_iterator_impl& impl) : mImpl(impl) {}
      clocked_peq_container_iterator_impl mImpl;
      friend class clocked_peq<PAYLOAD>;
      friend class clocked_peq_const_iterator<PAYLOAD>;
   };
   
   /** A pointer into a clocked_peq that allows to iterate in forward
    *  direction over the sorted list of the PEQ. This const iterator can
    *  not be used to remove the payload it is currently pointing to from
    *  the PEQ. Iterating with this const version of the iterator is
    *  slightly faster than iterating with the non-const version
    *  clocked_peq_iterator. Iterators are usually generated by the begin()
    *  and end() method of a container. */
   template <typename PAYLOAD>
   class clocked_peq_const_iterator {
    public:
      typedef clocked_peq_const_iterator<PAYLOAD> self;
      typedef clocked_peq_container_impl owner;

      /** Construct an iterator that does not point to any payload. */
      clocked_peq_const_iterator() : mImpl() {}
      /** Construct an const iterator from the non-const iterator X, that
       *  points to the same position. */
      clocked_peq_const_iterator(const clocked_peq_iterator<PAYLOAD>& x) : mImpl(x.mImpl) {}
      
      /** Return the payload pointed to by the iterator. */
      const PAYLOAD* operator*() const { return reinterpret_cast<const PAYLOAD*>(mImpl.get_payload()); }
      /** Return the payload pointed to by the iterator. */
      const PAYLOAD* operator->() const { return reinterpret_cast<const PAYLOAD*>(mImpl.get_payload()); }

      /** Move the pointer forward to the next entry in the PEQ. It returns
       *  an iterator that points to the new position. */
      self& operator++() { mImpl.increment(); return *this; }
      /** Move the pointer forward to the next entry in the PEQ. It returns
       *  an iterator that points to the old position. */
      self operator++(int) { self t=*this; mImpl.increment(); return t; }
      
      /** Compare the iterator to the other iterator X. It returns true if
       *  both iterators are pointing to the same position. */ 
      bool operator==(const self& x) const { return mImpl==x.mImpl; }
      /** Compare the iterator to the other iterator X. It returns true if
       *  both iterators are pointing to different positions. */ 
      bool operator!=(const self& x) const { return mImpl!=x.mImpl; }

    private:
      clocked_peq_const_iterator(const clocked_peq_container_const_iterator_impl& impl) : mImpl(impl) {}

      clocked_peq_container_const_iterator_impl mImpl;

      friend class clocked_peq<PAYLOAD>;
   };
   
}

#endif
// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
