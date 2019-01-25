/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_DMI_H
#define SCML2_MEMORY_DMI_H

#include <scml2_logging/snps_vp_dll.h>
#include <scml2_logging/stream.h>
#include <list>

namespace scml2 {


class SNPS_VP_API memory_dmi
{
public:
  struct DmiDataEntry
  {
    DmiDataEntry(unsigned long long startIndex,
                 unsigned long long endIndex,
                 bool dmiAllowed);
    unsigned long long startIndex;
    unsigned long long endIndex;
    bool dmiAllowed;
  };

  typedef std::list<DmiDataEntry> DmiData;
  typedef DmiData::iterator DmiDataIter;
  typedef DmiData::const_iterator DmiDataConstIter;

public:
  memory_dmi(unsigned long long size
	     );

  void clear();
  bool is_cleared() const;

  void disable();
  void disable(unsigned long long startIndex,
               unsigned long long endIndex);

  const DmiDataEntry& find(unsigned long long index) const;

  DmiDataConstIter begin() const;
  DmiDataConstIter end() const;
  unsigned long long nrOfRegions() const;

private:
  static logging::stream mInternalError;

private:
  unsigned long long mSize;
  bool mCleared;
  DmiData mDmiData;
};

inline
bool
memory_dmi::
is_cleared() const
{
  return mCleared;
}

inline
memory_dmi::DmiDataConstIter
memory_dmi::
begin() const
{
  return mDmiData.begin();
}

inline
memory_dmi::DmiDataConstIter
memory_dmi::
end() const
{
  return mDmiData.end();
}

inline
unsigned long long
memory_dmi::
nrOfRegions() const
{
  return mDmiData.size();
}

}

#endif
