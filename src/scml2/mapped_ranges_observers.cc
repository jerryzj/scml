/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/mapped_ranges_observers.h>
#include <scml2/mapped_ranges_observer_owner.h>
#include <cassert>

namespace scml2 {

mapped_ranges_observers::
mapped_ranges_observers() :
  mObservingEnabled(false),
  mObservers(new Observers),
  mMutableObservers(0),
  mNotifyLevel(0)
{
}

mapped_ranges_observers::
~mapped_ranges_observers()
{
  delete mObservers;
  assert(!mMutableObservers);
}

mapped_ranges_observers::Observers&
mapped_ranges_observers::
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
mapped_ranges_observers::
notify_mapped_ranges_changed()
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
      (*it)->get_owner()->handle_mapped_ranges_changed(*it);
    }
  }

  if (!--mNotifyLevel && mMutableObservers) {
    delete mObservers;
    mObservers = mMutableObservers;
    mMutableObservers = 0;
  }
}

bool
mapped_ranges_observers::
register_observer(mapped_ranges_observer* o)
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
mapped_ranges_observers::
unregister_all_observers()
{
  Observers& observers = get_mutable_observers();
  observers.clear();
  mObservingEnabled = false;
  return true;
}

bool
mapped_ranges_observers::
unregister_observer(mapped_ranges_observer* o)
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

}
