/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#ifndef SCML2_MEMORY_SLOW_ACCESS_CONTENT_OBSERVER_DECORATOR_H
#define SCML2_MEMORY_SLOW_ACCESS_CONTENT_OBSERVER_DECORATOR_H

#include <scml2/memory_slow_access.h>

#include <scml2/memory_content_observer.h>
#include <scml2/memory_content_observers.h>
#include <scml2/utils.h>
#include <scml2/types.h>

namespace scml2 {

template <typename DT, typename I>
class memory_slow_access_content_observer_decorator
{
public:
  memory_slow_access_content_observer_decorator(I impl,
                                                memory_content_observers& content_observers,
                                                unsigned long long offset, unsigned int memoryDataWidth);

  unsigned char* get_fast_backdoor() const;

  void put(unsigned long long address,
           const unsigned char* data,
           unsigned int dataLength,
           const unsigned char* byteEnablePtr,
           unsigned int byteEnableLength,
           bool isDebug = false);
  void get(unsigned long long address,
           unsigned char* data,
           unsigned int dataLength,
           const unsigned char* byteEnablePtr,
           unsigned int byteEnableLength,
           bool isDebug = false) const;
  void put(unsigned long long index,
           DT data,
           unsigned int size,
           unsigned int offset,
           bool isDebug = false);
  void put(unsigned long long index,
           DT data,
           bool isDebug = false);

  DT get(unsigned long long index,
         unsigned int size,
         unsigned int offset,
         bool isDebug = false) const;
  DT get(unsigned long long index,
         bool isDebug = false) const;

private:
  static access_mode get_mode(bool isDebug);

private:
  I mImpl;
  memory_content_observers& mContentObservers;
  const unsigned long long mOffset;
  const unsigned int mDataWidth;
};

template <typename DT, typename I>
inline
memory_slow_access_content_observer_decorator<DT, I>
create_slow_access_content_observer_decorator(I impl,
                                              memory_content_observers& content_observers,
                                              unsigned long long offset, unsigned int memoryDataWidth)
{
  return memory_slow_access_content_observer_decorator<DT, I>(impl, content_observers, offset, memoryDataWidth);
}

template <typename DT, typename I>
inline
memory_slow_access_content_observer_decorator<DT, I>::
memory_slow_access_content_observer_decorator(I impl,
                                              memory_content_observers& content_observers,
                                              unsigned long long offset, unsigned int memoryDataWidth) :
  mImpl(impl),
  mContentObservers(content_observers),
  mOffset(offset), mDataWidth(memoryDataWidth)
{
}

template <typename DT, typename I>
inline
access_mode
memory_slow_access_content_observer_decorator<DT, I>::
get_mode(bool isDebug)
{
  if (isDebug) {
    return ACCESS_MODE_DEBUG;

  } else {
    return ACCESS_MODE_REGULAR;
  }
}

template <typename DT, typename I>
inline
unsigned char*
memory_slow_access_content_observer_decorator<DT, I>::
get_fast_backdoor() const
{
  return 0;
}

template <typename DT, typename I>
inline
void
memory_slow_access_content_observer_decorator<DT, I>::
put(unsigned long long address,
    const unsigned char* data,
    unsigned int dataLength,
    const unsigned char* byteEnablePtr,
    unsigned int byteEnableLength,
    bool isDebug)
{
  mContentObservers.notify_observers(address_to_index<DT>(address) + mOffset,
                                     address_to_index<DT>(address + dataLength - 1) + mOffset,
                                     ACCESS_TYPE_WRITE,
                                     get_mode(isDebug),
                                     true);
  mImpl.put(address, data, dataLength, byteEnablePtr, byteEnableLength, isDebug);
  mContentObservers.notify_observers(address_to_index<DT>(address) + mOffset,
                                     address_to_index<DT>(address + dataLength - 1) + mOffset,
                                     ACCESS_TYPE_WRITE,
                                     get_mode(isDebug),
                                     false);
}

template <typename DT, typename I>
inline
void
memory_slow_access_content_observer_decorator<DT, I>::
get(unsigned long long address,
    unsigned char* data,
    unsigned int dataLength,
    const unsigned char* byteEnablePtr,
    unsigned int byteEnableLength,
    bool isDebug) const
{
  mContentObservers.notify_observers(address_to_index<DT>(address) + mOffset,
                                     address_to_index<DT>(address + dataLength - 1) + mOffset,
                                     ACCESS_TYPE_READ,
                                     get_mode(isDebug),
                                     true);
  mImpl.get(address, data, dataLength, byteEnablePtr, byteEnableLength, isDebug);
  mContentObservers.notify_observers(address_to_index<DT>(address) + mOffset,
                                     address_to_index<DT>(address + dataLength - 1) + mOffset,
                                     ACCESS_TYPE_READ,
                                     get_mode(isDebug),
                                     false);
}

template <typename DT, typename I>
inline
void
memory_slow_access_content_observer_decorator<DT, I>::
put(unsigned long long index,
    DT data,
    unsigned int size,
    unsigned int offset,
    bool isDebug)
{
  unsigned int wordIncr = (sizeof(DT) / mDataWidth);
  assert(wordIncr >= 1);
  mContentObservers.notify_observers(index * wordIncr + mOffset,
                                     index * wordIncr + mOffset,
                                     ACCESS_TYPE_WRITE,
                                     get_mode(isDebug),
                                     true);
  mImpl.put(index, data, size, offset, isDebug);
  mContentObservers.notify_observers(index * wordIncr + mOffset,
                                     index * wordIncr + mOffset,
                                     ACCESS_TYPE_WRITE,
                                     get_mode(isDebug),
                                     false);
}

template <typename DT, typename I>
inline
void
memory_slow_access_content_observer_decorator<DT, I>::
put(unsigned long long index,
    DT data,
    bool isDebug)
{
  unsigned int wordIncr = (sizeof(DT) / mDataWidth);
  assert(wordIncr >= 1);
  mContentObservers.notify_observers(index * wordIncr + mOffset,
                                     index * wordIncr + mOffset,
                                     sizeof(DT),
                                     ACCESS_TYPE_WRITE,
                                     get_mode(isDebug),
                                     true);
  mImpl.put(index, data, isDebug);
  mContentObservers.notify_observers(index * wordIncr + mOffset,
                                     index * wordIncr + mOffset,
                                     sizeof(DT),
                                     ACCESS_TYPE_WRITE,
                                     get_mode(isDebug),
                                     false);
}

template <typename DT, typename I>
inline
DT
memory_slow_access_content_observer_decorator<DT, I>::
get(unsigned long long index,
    unsigned int size,
    unsigned int offset,
    bool isDebug) const
{
  unsigned int wordIncr = (sizeof(DT) / mDataWidth);
  assert(wordIncr >= 1);
  mContentObservers.notify_observers(index * wordIncr + mOffset,
                                     index * wordIncr + mOffset,
                                     ACCESS_TYPE_READ,
                                     get_mode(isDebug),
                                     true);
  const DT result = mImpl.get(index, size, offset, isDebug);
  mContentObservers.notify_observers(index * wordIncr + mOffset,
                                     index * wordIncr + mOffset,
                                     ACCESS_TYPE_READ,
                                     get_mode(isDebug),
                                     false);
  return result;
}

template <typename DT, typename I>
inline
DT
memory_slow_access_content_observer_decorator<DT, I>::
get(unsigned long long index,
    bool isDebug) const
{
  unsigned int wordIncr = (sizeof(DT) / mDataWidth);
  assert(wordIncr >= 1);
  mContentObservers.notify_observers(index * wordIncr + mOffset,
                                     index * wordIncr + mOffset,
                                     ACCESS_TYPE_READ,
                                     get_mode(isDebug),
                                     true);
  const DT result = mImpl.get(index, isDebug);
  mContentObservers.notify_observers(index * wordIncr + mOffset,
                                     index * wordIncr + mOffset,
                                     ACCESS_TYPE_READ,
                                     get_mode(isDebug),
                                     false);
  return result;
}

}

#endif
