/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_STATUS_OBSERVER_H
#define SCML2_STATUS_OBSERVER_H

namespace scml2 {

class status_observer_owner;

class status_observer
{
public:
  status_observer(status_observer_owner* owner);
  virtual ~status_observer();

  status_observer_owner* get_owner() const;

private:
  status_observer_owner* mOwner;
};

inline
status_observer::
status_observer(status_observer_owner *owner) :
  mOwner(owner)
{
}

inline
status_observer::
~status_observer()
{
}

inline
status_observer_owner*
status_observer::
get_owner() const
{
  return mOwner;
}

}

#endif
