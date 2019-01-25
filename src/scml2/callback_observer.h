/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_CALLBACK_OBSERVER_H
#define SCML2_CALLBACK_OBSERVER_H

#include <scml2/types.h>
#include <scml2_logging/snps_vp_dll.h>

namespace scml2 {

class callback_observer_owner;

class SNPS_VP_API callback_observer
{
public:
  callback_observer(callback_observer_owner* owner,
                    access_type t,
                    access_mode m);
  virtual ~callback_observer();

  access_type get_access_type() const;
  void set_access_type(access_type t);
  access_mode get_access_mode() const;
  void set_access_mode(access_mode t);
  callback_observer_owner* get_owner() const;

private:
  access_type mAccessType;
  access_mode mAccessMode;
  callback_observer_owner* mOwner;
};

inline
callback_observer::
callback_observer(callback_observer_owner *owner,
                  access_type t,
                  access_mode m) :
  mAccessType(t),
  mAccessMode(m),
  mOwner(owner)
{
}

inline
callback_observer::
~callback_observer()
{
}
inline
access_type
callback_observer::
get_access_type() const
{
  return mAccessType;
}

inline
void
callback_observer::
set_access_type(access_type t)
{
  mAccessType = t;
}

inline
access_mode
callback_observer::
get_access_mode() const
{
  return mAccessMode;
}

inline
void
callback_observer::
set_access_mode(access_mode t)
{
  mAccessMode = t;
}

inline
callback_observer_owner*
callback_observer::
get_owner() const
{
  return mOwner;
}

}

#endif
