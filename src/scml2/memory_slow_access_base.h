/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_SLOW_ACCESS_BASE_H
#define SCML2_MEMORY_SLOW_ACCESS_BASE_H

#include <scml2/resource.h>

namespace scml2 {

template <typename DT>
class memory_slow_access_base 
  : public resource
{
public:
  virtual ~memory_slow_access_base();

  virtual unsigned char* get_fast_backdoor() const = 0;

  virtual void put(unsigned long long address,
                   const unsigned char* data,
                   unsigned int dataLength,
                   const unsigned char* byteEnablePtr,
                   unsigned int byteEnableLength,
                   bool isDebug = false) = 0;
  virtual void put(unsigned long long index,
                   DT data,
                   unsigned int size,
                   unsigned int offset,
                   bool isDebug = false) = 0;
  virtual void put(unsigned long long index,
                   DT data,
                   bool isDebug = false) = 0;
  virtual void get(unsigned long long address,
                   unsigned char* data,
                   unsigned int dataLength,
                   const unsigned char* byteEnablePtr,
                   unsigned int byteEnableLength,
                   bool isDebug = false) const = 0;
  virtual DT get(unsigned long long index,
                 unsigned int size,
                 unsigned int offset,
                 bool isDebug = false) const = 0;
  virtual DT get(unsigned long long index,
                 bool isDebug = false) const = 0;

  virtual void doAdd(unsigned long long index, DT rhs) = 0;
  virtual void doSubtract(unsigned long long index, DT rhs) = 0;
  virtual void doDivide(unsigned long long index, DT rhs) = 0;
  virtual void doMultiply(unsigned long long index, DT rhs) = 0;
  virtual void doModulo(unsigned long long index, DT rhs) = 0;
  virtual void doXor(unsigned long long index, DT rhs) = 0;
  virtual void doAnd(unsigned long long index, DT rhs) = 0;
  virtual void doOr(unsigned long long index, DT rhs) = 0;
  virtual void doShiftRight(unsigned long long index, DT rhs) = 0;
  virtual void doShiftLeft(unsigned long long index, DT rhs) = 0;

  virtual void doPreDecrement(unsigned long long index) = 0;
  virtual DT doPostDecrement(unsigned long long index) = 0;
  virtual void doPreIncrement(unsigned long long index) = 0;
  virtual DT doPostIncrement(unsigned long long index) = 0;
};

template <typename DT>
inline
memory_slow_access_base<DT>::
~memory_slow_access_base()
{
}

class memory_slow_access_wrapper_base 
{
public:
  virtual void put(unsigned long long address,
                   const unsigned char* data,
                   unsigned int dataLength,
                   const unsigned char* byteEnablePtr,
                   unsigned int byteEnableLength,
                   bool isDebug) = 0;
  virtual void get(unsigned long long address,
                   unsigned char* data,
                   unsigned int dataLength,
                   const unsigned char* byteEnablePtr,
                   unsigned int byteEnableLength,
                   bool isDebug) = 0;
  virtual ~memory_slow_access_wrapper_base() {}
};

template <typename I>
class memory_slow_access_wrapper
: public memory_slow_access_wrapper_base 
{
public:
  memory_slow_access_wrapper(I& impl) : mImpl(impl) {}
  virtual void put(unsigned long long address,
                   const unsigned char* data,
                   unsigned int dataLength,
                   const unsigned char* byteEnablePtr,
                   unsigned int byteEnableLength,
                   bool isDebug) 
  {
    mImpl.put(address, data, dataLength, byteEnablePtr, byteEnableLength, isDebug);
  }
  virtual void get(unsigned long long address,
                   unsigned char* data,
                   unsigned int dataLength,
                   const unsigned char* byteEnablePtr,
                   unsigned int byteEnableLength,
                   bool isDebug) 
  {
    mImpl.get(address, data, dataLength, byteEnablePtr, byteEnableLength, isDebug);
  }
 private:
  I& mImpl;
};

template<typename I>
memory_slow_access_wrapper<I> create_memory_slow_access_wrapper(I& impl)
{
  return memory_slow_access_wrapper<I>(impl);
}

}

#endif
