/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_RESOURCE_H
#define SCML2_RESOURCE_H

#include <cassert>

namespace scml2 {

class resource
{
public:
  resource();

  void ref() const;
  void unref() const;
  unsigned int getRefCount() const;

protected:
  virtual ~resource();

private:
  resource(const resource&);
  resource& operator=(const resource&);

private:
  mutable unsigned int mRefCount;
};

inline
resource::
resource() :
  mRefCount(0)
{
}

inline
resource::
~resource()
{
}

inline
void
resource::
ref() const
{
  ++mRefCount;
}

inline
void
resource::
unref() const
{
  assert(mRefCount);
  if (--mRefCount == 0) {
    delete this;
  }
}

inline
unsigned int
resource::
getRefCount() const
{
  return mRefCount;
}

}

#endif
