/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML_CALLBACK_OBSERVERS_H
#define SCML_CALLBACK_OBSERVERS_H

#include "scmlinc/scml_callback_observer.h"
#include "scmlinc/scml_types.h"
#include <scml2_logging/snps_vp_dll.h>
#include <PV/PV.h>
#include <set>

namespace sc_core {
  class sc_time;
}

class scml_callback_owner;

class SNPS_VP_API scml_callback_observers
{
public:
  scml_callback_observers();
  ~scml_callback_observers();

  bool has_observers() const;

  template <typename DT, typename AT>
  unsigned int notify_begin(const PVReq<DT, AT>& req) const;
  template <typename DT, typename AT>
  void notify_end(const PVReq<DT, AT>& req, unsigned int id) const;

  bool register_observer(scml_callback_observer* o);
  bool unregister_observer(scml_callback_observer* o);

private:
  typedef std::set<scml_callback_observer*> Observers;
  typedef Observers::iterator ObserversIter;
  typedef Observers::const_iterator ObserversConstIter;
  
  Observers& get_mutable_observers();
  void update_observers() const;

private:
  bool mObservingEnabled;
  mutable Observers* mObservers;
  mutable Observers* mMutableObservers;
  mutable int mNotifyLevel;
};

inline
bool
scml_callback_observers::
has_observers() const
{
  return mObservingEnabled;
}

#endif
