/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_RESTRICTION_OWNER_H
#define SCML2_RESTRICTION_OWNER_H

#include <scml2_logging/snps_vp_dll.h>

namespace scml2 {

class SNPS_VP_API restriction_owner {
public:
  restriction_owner();

  unsigned get_partial_read_restriction_count() const;
  unsigned get_partial_write_restriction_count() const;
protected:
  virtual restriction_owner* get_restriction_parent() const = 0;

  void read_restriction_removed();
  void read_restriction_added();
  void write_restriction_removed();
  void write_restriction_added();
private:
  unsigned mReadRestrictionCount;
  unsigned mWriteRestrictionCount;
};

inline
unsigned 
restriction_owner::
get_partial_read_restriction_count() const
{
  return mReadRestrictionCount;
}

inline
unsigned 
restriction_owner::
get_partial_write_restriction_count() const
{
  return mWriteRestrictionCount;
}

}
#endif
