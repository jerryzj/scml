/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_SCOPED_REFERENCE_H
#define SCML2_SCOPED_REFERENCE_H

namespace scml2 {

template <typename T>
class scoped_reference
{
public:
  scoped_reference(T* t);
  ~scoped_reference();

private:
  T* mT;
};

template <typename T>
inline
scoped_reference<T>::
scoped_reference(T* t) :
  mT(t)
{
  mT->ref();
}

template <typename T>
inline
scoped_reference<T>::
~scoped_reference()
{
  mT->unref();
}

}

#endif
