/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/status.h>
#include <scml2/status_observer.h>
#include <scml2/status_observer_owner.h>
#include <scml2/status_registry.h>

namespace scml2 {

status::
status(const std::string& name) :
  sc_core::sc_object(name.c_str())
{
  bool result = status_registry::get_instance().register_status(this);
  assert(result); (void)result;
}

status::
~status()
{
  bool result = status_registry::get_instance().unregister_status(this);
  assert(result); (void)result;
}

void
status::
set_status(const std::string& status)
{
  if (mStatus != status) {
    mStatus = status;
    notify_observers();
  }
}

void
status::
set_description(const std::string& d)
{
  mDescription = d;
}

const std::string&
status::
get_description() const
{
  return mDescription;
}

void
status::
notify_observers() const
{
  ObserversConstIter it = mObservers.begin();
  ObserversConstIter end = mObservers.end();
  for (; it != end; ++it) {
    (*it)->get_owner()->handle_status_changed(*it);
  }
}

bool
status::
register_observer(status_observer* o)
{
  if (mObservers.find(o) != mObservers.end()) {
    return false;
  }
  mObservers.insert(o);
  return true;
}

bool
status::
unregister_all_observers()
{
  mObservers.clear();
  return true;
}

bool
status::
unregister_observer(status_observer* o)
{
  ObserversIter it = mObservers.find(o);
  if (it == mObservers.end()) {
    return false;
  }
  mObservers.erase(it);
  return true;
}

}
