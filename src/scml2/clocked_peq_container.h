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
#ifndef SCML2_CLOCKED_PEQ_CONTAINER_H
#define SCML2_CLOCKED_PEQ_CONTAINER_H

#include "clocked_peq_container_impl.h"

namespace scml2 {
   
   template <typename PAYLOAD> class clocked_peq_container_iterator;
   template <typename PAYLOAD> class clocked_peq_container_const_iterator;
   
   /** A clocked payload event queue (PEQ) container is a convenience
    *  modeling object for TLM2 models with temporal decoupling. It can be
    *  used to buffer input payload that was received via a temporal
    *  decoupled transport. Each payload that is pushed into the container
    *  is tagged by a timestamp that corresponds to the local time of the
    *  arrival time. The time stamp is first converted to the tick count of
    *  the associated clock, before it is saved in the container.
    *
    *  The container sorts the payload by its timestamp. The container
    *  reflects the view of a user that lives in the current SystemC
    *  time. It only shows payload that arrived in the past. That means
    *  that all payload with timestamp >= get_clock()->get_tick_count() is
    *  hidden from the user interface. For payload that arrived in the
    *  past, the container behaves like a single-linked list that is sorted
    *  by the timestamp.
    *  
    *  Because the timestamp for payload is based on clock tick count and
    *  not SystemC time, the arrival time of all future payload is always
    *  adapted in case of parameter changes of the clock like
    *  period/enable/disable.
    *
    *  The container provides iterators for iterating over the sorted list
    *  of the container in forward direction. The API of the iterators
    *  conforms to the STL. There is a const and a non-const version of the
    *  iterator. The const iterator provides a faster iteration
    *  mechanism. The non-const iterator allows to remove the payload it is
    *  pointing to from the list. */
   template <typename PAYLOAD>
   class clocked_peq_container {
    public:
      typedef clocked_peq_container_iterator<PAYLOAD> iterator;
      typedef clocked_peq_container_const_iterator<PAYLOAD> const_iterator;

      /** Construct a clocked PEQ container that is associated with the
       *  SCML2 clock object CLOCK. */ 
      clocked_peq_container(scml_clock_if* clock=0) : mImpl() { set_clock(clock); }
      /** Construct a clocked PEQ container that is associated with the
       *  SCML clock, which is bound to the provided port P. */ 
      clocked_peq_container(sc_core::sc_in<bool>& p) : mImpl() { set_clock(scml2::get_scml_clock(p)); }

      /** Associates the clocked_peq_container with the provided SCML2
       *  clock CLOCK. */
      void set_clock(scml_clock_if* clock) { mImpl.set_clock(clock); }
      /** Associates the clocked_peq_container with the SCML clock, which
       *  is bound to the provided port P. */
      void set_clock(sc_core::sc_in<bool>& p) { set_clock(scml2::get_scml_clock(p)); }
      /** Returns the associated SCML2 clock. */
      scml_clock_if* get_clock() const { return mImpl.get_clock(); }
      
      /** Push a payload into the container that was received by a temporal
       *  decoupled transport at local time ARRIVAL_LOCAL_TIME. The local
       *  time argument is given as SystemC time. It is converted to the
       *  clock tick count by rounding down to the last clock tick. */
      bool notify(PAYLOAD* payload, const sc_core::sc_time& arrival_local_time) { return mImpl.notify(payload, arrival_local_time); }
      /** Push a payload into the container that was received by a temporal
       *  decoupled transport. The local time is defined by two arguments,
       *  the SystemC time ARRIVAL_LOCAL_TIME and the additional number of
       *  clock ticks DELAY_TICKS. The internal arrival clock tick count is
       *  calculated by first converting ARRIVAL_LOCAL_TIME to the clock
       *  tick count by rounding down to the last clock tick, and then
       *  incrementing it by DELAY_TICKS. */
      bool notify(PAYLOAD* payload, const sc_core::sc_time& arrival_local_time, long long delay_ticks) {
	 return mImpl.notify(payload, arrival_local_time, delay_ticks);
      }
      /** Push a payload into the container that was received by a temporal
       *  decoupled transport at the local time that is CLOCK_TICKS_TO_SKIP
       *  clock ticks in the future. */
      bool notify(void* payload, unsigned long long clock_ticks_to_skip) { return mImpl.notify(payload, clock_ticks_to_skip); }
      /** Push a payload into the container that was received by a temporal
       *  decoupled transport at the local time represented by the clock
       *  tick count ARRIVAL_TICK_COUNT. */
      bool notifyAt(void* payload, unsigned long long arrival_tick_count) { return mImpl.notifyAt(payload, arrival_tick_count); }

      /** Return true if the container is empty. This only considers
       *  visible payload (timestamp < get_clock()->get_tick_count()). */
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
       *  returns NULL if the list is empty. This only considers visible
       *  payload (timestamp < get_clock()->get_tick_count()). */
      PAYLOAD* get_next() { return reinterpret_cast<PAYLOAD*>(mImpl.get_next()); }

      /** Remove the payload at the position which is given by the iterator
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
       *  contrast to the method is_empty() it considers not only the
       *  visible but all payload. This method is usually used if all
       *  visible/past payload has been processed/removed, in order to
       *  check if more processing has to be done at a future time. */
      bool has_more_events() const { return mImpl.has_more_events(); }
      /** Return the arrival clock tick of the next payload in the sorted
       *  list of the container. This method must not be called if
       *  has_more_events() returned false!
       *
       *  Note that this method does not only consider the visible but all
       *  payload. This method is usually used when all current processing
       *  has been done, in order to retrieve the time for which more
       *  processing shall be scheduled. */
      unsigned long long get_next_event_arrival_tick() const { return mImpl.get_next_event_arrival_tick(); }
      
    private:
      clocked_peq_container_impl mImpl;
   };
   
   /** A pointer into a clocked_peq_container that allows to iterate in
    *  forward direction over the sorted list of the container. This
    *  non-const iterator can be used to remove the payload it is currently
    *  pointing to from the container. Iterating with this non-const
    *  version of the iterator is slightly slower than iterating with the
    *  const version clocked_peq_container_const_iterator. Iterators are
    *  usually generated by the begin() and end() method of a container. */
   template <typename PAYLOAD>
   class clocked_peq_container_iterator {
    public:
      typedef clocked_peq_container_iterator<PAYLOAD> self;
      typedef clocked_peq_container_impl owner;

      /** Construct an iterator that does not point to any payload. */
      clocked_peq_container_iterator() : mImpl() {}

      /** Return the payload pointed to by the iterator. */
      PAYLOAD* operator*() const { return reinterpret_cast<PAYLOAD*>(mImpl.get_payload()); }
      /** Return the payload pointed to by the iterator. */
      PAYLOAD* operator->() const { return reinterpret_cast<PAYLOAD*>(mImpl.get_payload()); }

      /** Move the pointer forward to the next entry in the container.
       *  Returns an iterator that points to the new position. */
      self& operator++() { mImpl.increment(); return *this; }
      /** Move the pointer forward to the next entry in the container. It
       *  returns an iterator that points to the old position. */
      self operator++(int) { self t=*this; mImpl.increment(); return t; }
      
      /** Compare the iterator to the other iterator X. Is returns true if
       *  both iterators are pointing to the same position. */ 
      bool operator==(const self& x) const { return mImpl==x.mImpl; }
      /** Compare the iterator to the other iterator X. It returns true if
       *  both iterators are pointing to different positions. */ 
      bool operator!=(const self& x) const { return mImpl!=x.mImpl; }

    private:
      clocked_peq_container_iterator(const clocked_peq_container_iterator_impl& impl) : mImpl(impl) {}
      clocked_peq_container_iterator_impl mImpl;
      friend class clocked_peq_container<PAYLOAD>;
      friend class clocked_peq_container_const_iterator<PAYLOAD>;
   };
   
   /** A pointer into a clocked_peq_container that allows to iterate in
    *  forward direction over the sorted list of the container. This const
    *  iterator can not be used to remove the payload it is currently
    *  pointing to from the container. Iterating with this const version of
    *  the iterator is slightly faster than iterating with the non-const
    *  version clocked_peq_container_iterator. Iterators are usually
    *  generated by the begin() and end() method of a container. */
   template <typename PAYLOAD>
   class clocked_peq_container_const_iterator {
    public:
      typedef clocked_peq_container_const_iterator<PAYLOAD> self;
      typedef clocked_peq_container_impl owner;

      /** Construct an iterator that does not point to any payload. */
      clocked_peq_container_const_iterator() : mImpl() {}
      /** Construct an const iterator from the non-const iterator X, that
       *  points to the same position. */
      clocked_peq_container_const_iterator(const clocked_peq_container_iterator<PAYLOAD>& x) : mImpl(x.mImpl) {}
      
      /** Return the payload pointed to by the iterator. */
      const PAYLOAD* operator*() const { return reinterpret_cast<const PAYLOAD*>(mImpl.get_payload()); }
      /** Return the payload pointed to by the iterator. */
      const PAYLOAD* operator->() const { return reinterpret_cast<const PAYLOAD*>(mImpl.get_payload()); }

      /** Move the pointer forward to the next entry in the container.  It
       *  returns an iterator that points to the new position. */
      self& operator++() { mImpl.increment(); return *this; }
      /** Move the pointer forward to the next entry in the container. It
       *  returns an iterator that points to the old position. */
      self operator++(int) { self t=*this; mImpl.increment(); return t; }
      
      /** Compare the iterator to the other iterator X. It returns true if
       *  both iterators are pointing to the same position. */ 
      bool operator==(const self& x) const { return mImpl==x.mImpl; }
      /** Compare the iterator to the other iterator X. It returns true if
       *  both iterators are pointing to different positions. */ 
      bool operator!=(const self& x) const { return mImpl!=x.mImpl; }

    private:
      clocked_peq_container_const_iterator(const clocked_peq_container_const_iterator_impl& impl) : mImpl(impl) {}
      clocked_peq_container_const_iterator_impl mImpl;
      friend class clocked_peq_container<PAYLOAD>;
   };

}

#endif
// {{{ Emacs local variables

// Local variables:
// folded-file: t
// c-file-style: "coware"
// End:

// }}}
