/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MAPPED_RANGES_OBSERVERS_H
#define SCML2_MAPPED_RANGES_OBSERVERS_H

#include <scml2/mapped_ranges_observer.h>
#include <scml2_logging/snps_vp_dll.h>

#include <set>

namespace scml2 {

class mapped_ranges_observer_owner;

class SNPS_VP_API mapped_ranges_observers
{
public:
  mapped_ranges_observers();
  ~mapped_ranges_observers();

  bool has_observers() const;

  void notify_mapped_ranges_changed();

  bool register_observer(mapped_ranges_observer* o);
  bool unregister_all_observers();
  bool unregister_observer(mapped_ranges_observer* o);

private:
  typedef std::set<mapped_ranges_observer*> Observers;
  typedef Observers::iterator ObserversIter;
  typedef Observers::const_iterator ObserversConstIter;

  Observers& get_mutable_observers();

private:
  bool mObservingEnabled;
  mutable Observers* mObservers;
  mutable Observers* mMutableObservers;
  mutable int mNotifyLevel;
};

inline
bool
mapped_ranges_observers::
has_observers() const
{
  return mObservingEnabled;
}

}

#endif
