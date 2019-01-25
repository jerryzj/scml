/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_QUANTUMKEEPER_WRAPPER_H
#define SCML2_QUANTUMKEEPER_WRAPPER_H

#include <scml2/quantumkeeper_if.h>

namespace scml2 {

template <typename T>
class quantumkeeper_wrapper : public quantumkeeper_if
{
public:
  quantumkeeper_wrapper(T& quantumkeeper);

  virtual void inc(const sc_core::sc_time& t);
  virtual void set(const sc_core::sc_time& t);
  virtual sc_core::sc_time get_local_time() const;

private:
  T& mImpl;
};

template <typename T>
inline
quantumkeeper_wrapper<T>::
quantumkeeper_wrapper(T& quantumkeeper) :
 mImpl(quantumkeeper)
{
}

template <typename T>
inline
void
quantumkeeper_wrapper<T>::
inc(const sc_core::sc_time& t)
{
  mImpl.inc(t);
}

template <typename T>
inline
void
quantumkeeper_wrapper<T>::
set(const sc_core::sc_time& t)
{
  mImpl.set(t);
}

template <typename T>
inline
sc_core::sc_time
quantumkeeper_wrapper<T>::
get_local_time() const
{
  return mImpl.get_local_time();
}

}

#endif
