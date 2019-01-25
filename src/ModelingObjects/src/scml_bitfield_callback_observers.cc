/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#include "scml_bitfield_callback_observers.h"
#include "scml_bitfield_callback_observer_owner.h"
#include <systemc>
#include <cassert>

scml_bitfield_callback_observers::
scml_bitfield_callback_observers() :
  mObservingEnabled(false),
  mObservers(new Observers),
  mMutableObservers(0),
  mNotifyLevel(0)
{
}

scml_bitfield_callback_observers::
~scml_bitfield_callback_observers()
{
  delete mObservers;
  assert(!mMutableObservers);
}

scml_bitfield_callback_observers::Observers&
scml_bitfield_callback_observers::
get_mutable_observers()
{
  Observers* observers = mObservers;
  if (mNotifyLevel) {
    if (!mMutableObservers) {
      // lazy creation of copy (aka copy-on-write)
      mMutableObservers = new Observers(*observers);
    }
    observers = mMutableObservers;
  }
  return *observers;
}

void
scml_bitfield_callback_observers::
update_observers() const
{
  if (!--mNotifyLevel && mMutableObservers) {
    delete mObservers;
    mObservers = mMutableObservers;
    mMutableObservers = 0;
  }
}

unsigned int
scml_bitfield_callback_observers::
notify_begin(const unsigned int* data, scml::AccessType type, scml::AccessMode mode) const
{
  if (!mObservingEnabled) {
    return (unsigned int)-1;
  }

  static unsigned int nextId = 0;

  const unsigned int currentId = nextId;
  if (++nextId == (unsigned int)-1) {
    nextId = 0;
  }

  ++mNotifyLevel;

  ObserversConstIter it = mObservers->begin();
  ObserversConstIter end = mObservers->end();
  for (; it != end; ++it) {
    if (mMutableObservers == 0 ||
        mMutableObservers->find(*it) != mMutableObservers->end()) {
      if ((*it)->get_access_type() & type &&
          (*it)->get_access_mode() & mode) {
        (*it)->get_owner()->handle_begin_callback(*it, data, type, mode, currentId);
      }
    }
  }
  
  update_observers();
  return currentId;
}

void
scml_bitfield_callback_observers::
notify_end(const unsigned int* data, unsigned int id) const
{
  if (!mObservingEnabled) {
    return;
  }

  ++mNotifyLevel;

  ObserversConstIter it = mObservers->begin();
  ObserversConstIter end = mObservers->end();
  for (; it != end; ++it) {
    if (mMutableObservers == 0 ||
        mMutableObservers->find(*it) != mMutableObservers->end()) {
      (*it)->get_owner()->handle_end_callback(*it, data, id);
    }
  }

  update_observers();
}

bool
scml_bitfield_callback_observers::
register_observer(scml_bitfield_callback_observer* o)
{
  Observers& observers = get_mutable_observers();
  if (observers.find(o) != observers.end()) {
    return false;
  }
  mObservingEnabled = true;
  observers.insert(o);
  return true;
}

bool
scml_bitfield_callback_observers::
unregister_observer(scml_bitfield_callback_observer* o)
{
  Observers& observers = get_mutable_observers();
  ObserversIter it = observers.find(o);
  if (it == observers.end()) {
    return false;
  }
  observers.erase(it);
  if (observers.empty()) {
    mObservingEnabled = false;
  }
  return true;
}
