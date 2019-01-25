/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML_MAPPED_RANGES_OBSERVER_H
#define SCML_MAPPED_RANGES_OBSERVER_H

class scml_mapped_ranges_observer_owner;

class scml_mapped_ranges_observer
{
public:
  scml_mapped_ranges_observer(scml_mapped_ranges_observer_owner* owner);
  virtual ~scml_mapped_ranges_observer();

  scml_mapped_ranges_observer_owner* get_owner() const;

private:
  scml_mapped_ranges_observer_owner* mOwner;
};

inline
scml_mapped_ranges_observer::
scml_mapped_ranges_observer(scml_mapped_ranges_observer_owner *owner) :
  mOwner(owner)
{
}

inline
scml_mapped_ranges_observer::
~scml_mapped_ranges_observer()
{
}

inline
scml_mapped_ranges_observer_owner*
scml_mapped_ranges_observer::
get_owner() const
{
  return mOwner;
}

#endif
