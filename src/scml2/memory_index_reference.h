/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_INDEX_REFERENCE_H
#define SCML2_MEMORY_INDEX_REFERENCE_H

#include <scml2/memory.h>

#include <cassert>

namespace scml2 {

template <typename DT> class memory;

template <typename DT>
class memory_index_reference
{
public:
  typedef DT data_type;
  typedef memory_index_reference<DT> reference;

public:
  memory_index_reference(toplevel_memory_base* m, unsigned long long index);
  memory_index_reference(toplevel_memory_base* m, unsigned long long offset, unsigned long long index);
  memory_index_reference(const memory_index_reference<DT>& other);

  void put(const DT& value);
  DT get() const;

  void put_debug(const DT& value);
  DT get_debug() const;

  operator DT() const;

  reference& operator=(DT value);

  reference& operator+=(DT value);
  reference& operator-=(DT value);
  reference& operator/=(DT value);
  reference& operator*=(DT value);
  reference& operator%=(DT value);
  reference& operator^=(DT value);
  reference& operator&=(DT value);
  reference& operator|=(DT value);
  reference& operator>>=(DT value);
  reference& operator<<=(DT value);

  reference& operator--();
  DT operator--(int);
  reference& operator++();
  DT operator++(int);

protected:
  toplevel_memory_base* mMemory;
  memory_fast_access<DT> mFastAccess;
  unsigned long long mIndex;
};

template <typename DT>
inline
memory_index_reference<DT>::
memory_index_reference(toplevel_memory_base* m, unsigned long long index) :
  mMemory(m),
  mIndex(index)
{
  mFastAccess.update(mMemory->template create_slow_access<DT>(0));
}

template <typename DT>
inline
memory_index_reference<DT>::
memory_index_reference(toplevel_memory_base* m, unsigned long long offset, unsigned long long index) :
  mMemory(m),
  mIndex(index)
{
  mFastAccess.update(mMemory->template create_slow_access<DT>(offset));
}

template <typename DT>
inline
memory_index_reference<DT>::
memory_index_reference(const memory_index_reference<DT>& other) :
  mMemory(other.mMemory),
  mFastAccess(),
  mIndex(other.mIndex)
{
  mFastAccess.update(other.mFastAccess.mSlowAccess);
}

template <typename DT>
inline
void
memory_index_reference<DT>::
put(const DT& value)
{
  assert(mMemory);
  mFastAccess.put(mIndex, value, false);
}

template <typename DT>
inline
DT
memory_index_reference<DT>::
get() const
{
  assert(mMemory);
  return mFastAccess.get(mIndex, false);
}

template <typename DT>
inline
void
memory_index_reference<DT>::
put_debug(const DT& value)
{
  assert(mMemory);
  mFastAccess.put_debug(mIndex, value, true);
}

template <typename DT>
inline
DT
memory_index_reference<DT>::
get_debug() const
{
  assert(mMemory);
  return mFastAccess.get_debug(mIndex, true);
}

template <typename DT>
inline
memory_index_reference<DT>::
operator DT() const
{
  return get();
}

template <typename DT>
inline
memory_index_reference<DT>&
memory_index_reference<DT>::
operator=(DT value)
{
  put(value);
  return *this;
}

template <typename DT>
inline
memory_index_reference<DT>&
memory_index_reference<DT>::
operator+=(DT value)
{
  assert(mMemory);
  mFastAccess.doAdd(mIndex, value);
  return *this;
}

template <typename DT>
inline
memory_index_reference<DT>&
memory_index_reference<DT>::
operator-=(DT value)
{
  assert(mMemory);
  mFastAccess.doSubtract(mIndex, value);
  return *this;
}

template <typename DT>
inline
memory_index_reference<DT>&
memory_index_reference<DT>::
operator/=(DT value)
{
  assert(mMemory);
  mFastAccess.doDivide(mIndex, value);
  return *this;
}

template <typename DT>
inline
memory_index_reference<DT>&
memory_index_reference<DT>::
operator*=(DT value)
{
  assert(mMemory);
  mFastAccess.doMultiply(mIndex, value);
  return *this;
}

template <typename DT>
inline
memory_index_reference<DT>&
memory_index_reference<DT>::
operator%=(DT value)
{
  assert(mMemory);
  mFastAccess.doModulo(mIndex, value);
  return *this;
}

template <typename DT>
inline
memory_index_reference<DT>&
memory_index_reference<DT>::
operator^=(DT value)
{
  assert(mMemory);
  mFastAccess.doXor(mIndex, value);
  return *this;
}

template <typename DT>
inline
memory_index_reference<DT>&
memory_index_reference<DT>::
operator&=(DT value)
{
  assert(mMemory);
  mFastAccess.doAnd(mIndex, value);
  return *this;
}

template <typename DT>
inline
memory_index_reference<DT>&
memory_index_reference<DT>::
operator|=(DT value)
{
  assert(mMemory);
  mFastAccess.doOr(mIndex, value);
  return *this;
}

template <typename DT>
inline
memory_index_reference<DT>&
memory_index_reference<DT>::
operator>>=(DT value)
{
  assert(mMemory);
  mFastAccess.doShiftRight(mIndex, value);
  return *this;
}

template <typename DT>
inline
memory_index_reference<DT>&
memory_index_reference<DT>::
operator<<=(DT value)
{
  assert(mMemory);
  mFastAccess.doShiftLeft(mIndex, value);
  return *this;
}

template <typename DT>
inline
memory_index_reference<DT>&
memory_index_reference<DT>::
operator--()
{
  assert(mMemory);
  mFastAccess.doPreDecrement(mIndex);
  return *this;
}

template <typename DT>
inline
DT
memory_index_reference<DT>::
operator--(int)
{
  assert(mMemory);
  return mFastAccess.doPostDecrement(mIndex);
}

template <typename DT>
inline
memory_index_reference<DT>&
memory_index_reference<DT>::
operator++()
{
  assert(mMemory);
  mFastAccess.doPreIncrement(mIndex);
  return *this;
}

template <typename DT>
inline
DT
memory_index_reference<DT>::
operator++(int)
{
  assert(mMemory);
  return mFastAccess.doPostIncrement(mIndex);
}

}

#endif
