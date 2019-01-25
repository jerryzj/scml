/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML_MAPPED_RANGE_H
#define SCML_MAPPED_RANGE_H

#include "scmlinc/scml_mapable_if.h"

class scml_mapped_range
{
public:
  typedef unsigned long long size_type;

public:
  scml_mapped_range(size_type b, size_type s, scml_mapable_if& d, size_type o) :
    mBase(b),
    mSize(s),
    mDest(d),
    mOffset(o)
  {
  }
  
  size_type get_base() const { return mBase; }
  sc_dt::uint64 get_size() const { return mSize; }
  size_type get_offset() const { return mOffset; }
  
  bool contains(size_type address) const
  {
    return (mBase <= address && address <= (mBase + (size_type) (mSize - 1)));
  }
  
  sc_dt::uint64 mapped_address(sc_dt::uint64 address) const {
    return ((address - mBase) + mOffset);
  }
  
  // forward
  const char* get_mapped_name() const { return mDest.getName(); }
  template <typename IF>
  typename IF::response_type transport(const typename IF::request_type& req)
  {
    return mDest.transport(req);
  }
  
private:
  size_type mBase;
  sc_dt::uint64 mSize;
  scml_mapable_if& mDest;
  size_type mOffset;
};

#endif
