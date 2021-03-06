/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/status_registry.h>
#include <scml2/status.h>

#include <cassert>
#include <limits>

namespace scml2 {

status_registry&
status_registry::
get_instance()
{
  static status_registry instance;
  return instance;
}

status_registry::
status_registry()
{
}

status_registry::
~status_registry()
{
}

bool
status_registry::
register_status(status* m)
{
  assert(m);
  if (find_status_by_name(m->get_name())) {
    return false;
  }
  mStates.insert(m);
  return true;
}

bool
status_registry::
unregister_status(status* m)
{
  return (mStates.erase(m) != 0);
}

status*
status_registry::
find_status_by_name(const std::string& name) const
{
  StatesConstIter it = mStates.begin();
  StatesConstIter end = mStates.end();
  for (; it != end; ++it) {
    if ((*it)->get_name() == name) {
      return *it;
    }
  }
  return 0;
}

unsigned int
status_registry::
collect_statuses(std::vector<status*>& v) const
{
  StatesConstIter it = mStates.begin();
  StatesConstIter end = mStates.end();
  for (; it != end; ++it) {
    v.push_back(*it);
  }
  size_t size =  mStates.size();
  assert( size < std::numeric_limits<unsigned int>::max());
  return (unsigned int)size;
}

}
