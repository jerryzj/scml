/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/restriction_owner.h>

#include <cassert>

namespace scml2 {

restriction_owner::
restriction_owner()
  : mReadRestrictionCount(0)
  , mWriteRestrictionCount(0)
{
}

void 
restriction_owner::
read_restriction_removed()
{
  assert(mReadRestrictionCount != 0);
  --mReadRestrictionCount;
  if (get_restriction_parent() != 0) {
    get_restriction_parent()->read_restriction_removed();
  }
}

void 
restriction_owner::
read_restriction_added()
{
  ++mReadRestrictionCount;
  if (get_restriction_parent() != 0) {
    get_restriction_parent()->read_restriction_added();
  }
}

void 
restriction_owner::
write_restriction_removed()
{
  assert(mWriteRestrictionCount != 0);
  --mWriteRestrictionCount;
  if (get_restriction_parent() != 0) {
    get_restriction_parent()->write_restriction_removed();
  }
}

void 
restriction_owner::
write_restriction_added()
{
  ++mWriteRestrictionCount;
  if (get_restriction_parent() != 0) {
    get_restriction_parent()->write_restriction_added();
  }
}

}
