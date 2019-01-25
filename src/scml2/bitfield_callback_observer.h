/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_CALLBACK_OBSERVER_H
#define SCML2_BITFIELD_CALLBACK_OBSERVER_H

#include <scml2/types.h>

namespace scml2 {

template <typename DT> class bitfield_callback_observer_owner;

template <typename DT>
class bitfield_callback_observer
{
public:
  bitfield_callback_observer(bitfield_callback_observer_owner<DT>* owner,
                             access_type t,
                             access_mode m);
  virtual ~bitfield_callback_observer();

  access_type get_access_type() const;
  void set_access_type(access_type t);
  access_mode get_access_mode() const;
  void set_access_mode(access_mode t);
  bitfield_callback_observer_owner<DT>* get_owner() const;

private:
  access_type mAccessType;
  access_mode mAccessMode;
  bitfield_callback_observer_owner<DT>* mOwner;
};

template <typename DT>
inline
bitfield_callback_observer<DT>::
bitfield_callback_observer(bitfield_callback_observer_owner<DT>* owner,
                           access_type t,
                           access_mode m) :
  mAccessType(t),
  mAccessMode(m),
  mOwner(owner)
{
}

template <typename DT>
inline
bitfield_callback_observer<DT>::
~bitfield_callback_observer()
{
}

template <typename DT>
inline
access_type
bitfield_callback_observer<DT>::
get_access_type() const
{
  return mAccessType;
}

template <typename DT>
inline
void
bitfield_callback_observer<DT>::
set_access_type(access_type t)
{
  mAccessType = t;
}

template <typename DT>
inline
access_mode
bitfield_callback_observer<DT>::
get_access_mode() const
{
  return mAccessMode;
}

template <typename DT>
inline
void
bitfield_callback_observer<DT>::
set_access_mode(access_mode t)
{
  mAccessMode = t;
}

template <typename DT>
inline
bitfield_callback_observer_owner<DT>*
bitfield_callback_observer<DT>::
get_owner() const
{
  return mOwner;
}

}

#endif
