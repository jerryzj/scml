/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML_BITFIELD_CALLBACK_OBSERVER_H
#define SCML_BITFIELD_CALLBACK_OBSERVER_H

#include "scmlinc/scml_types.h"

class scml_bitfield_callback_observer_owner;

class scml_bitfield_callback_observer
{
public:
  scml_bitfield_callback_observer(scml_bitfield_callback_observer_owner* owner,
                                  scml::AccessType t,
                                  scml::AccessMode m);
  virtual ~scml_bitfield_callback_observer();

  scml::AccessType get_access_type() const;
  void set_access_type(scml::AccessType t);
  scml::AccessMode get_access_mode() const;
  void set_access_mode(scml::AccessMode t);
  scml_bitfield_callback_observer_owner* get_owner() const;

private:
  scml::AccessType mAccessType;
  scml::AccessMode mAccessMode;
  scml_bitfield_callback_observer_owner* mOwner;
};

inline
scml_bitfield_callback_observer::
scml_bitfield_callback_observer(scml_bitfield_callback_observer_owner *owner,
                                scml::AccessType t,
                                scml::AccessMode m) :
  mAccessType(t),
  mAccessMode(m),
  mOwner(owner)
{
}

inline
scml_bitfield_callback_observer::
~scml_bitfield_callback_observer()
{
}
inline
scml::AccessType
scml_bitfield_callback_observer::
get_access_type() const
{
  return mAccessType;
}

inline
void
scml_bitfield_callback_observer::
set_access_type(scml::AccessType t)
{
  mAccessType = t;
}

inline
scml::AccessMode
scml_bitfield_callback_observer::
get_access_mode() const
{
  return mAccessMode;
}

inline
void
scml_bitfield_callback_observer::
set_access_mode(scml::AccessMode t)
{
  mAccessMode = t;
}

inline
scml_bitfield_callback_observer_owner*
scml_bitfield_callback_observer::
get_owner() const
{
  return mOwner;
}

#endif
