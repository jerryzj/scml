/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MAPPED_RANGES_OBSERVER_H
#define SCML2_MAPPED_RANGES_OBSERVER_H

namespace scml2 {

class mapped_ranges_observer_owner;

class mapped_ranges_observer
{
public:
  mapped_ranges_observer(mapped_ranges_observer_owner* owner);
  virtual ~mapped_ranges_observer();

  mapped_ranges_observer_owner* get_owner() const;

private:
  mapped_ranges_observer_owner* mOwner;
};

inline
mapped_ranges_observer::
mapped_ranges_observer(mapped_ranges_observer_owner *owner) :
  mOwner(owner)
{
}

inline
mapped_ranges_observer::
~mapped_ranges_observer()
{
}

inline
mapped_ranges_observer_owner*
mapped_ranges_observer::
get_owner() const
{
  return mOwner;
}

}

#endif
