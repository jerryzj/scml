/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#include <scml2_logging/streambuf.h>
#include <scml2_logging/registry.h>
#include <limits>
#include <cassert>

#define INITIAL_BUFFER_SIZE 120

namespace scml2 { namespace logging {

streambuf::
streambuf(const stream_base& streamBase) :
  std::streambuf(),
  mBuffer(INITIAL_BUFFER_SIZE),
  mStreamBase(streamBase)
{
  setp(begin(), end());
}

std::streamsize
streambuf::
xsputn(const char* p, std::streamsize n)
{
  if (pptr() + n >= epptr()) {
    std::streamsize offset = pptr() - pbase();
    std::streamsize newSize = mBuffer.size() + n - (epptr() - pptr());
    mBuffer.resize((unsigned int)newSize);
    setp(begin(), end());
    assert(offset < std::numeric_limits<int>::max());
    pbump((int)offset);
  }

  std::copy(p, p + n, pptr());
  assert(n < std::numeric_limits<int>::max());
  pbump((int)n);

  return n;
}

int
streambuf::
sync()
{
  if (pptr() + 1 >= epptr()) {
    mBuffer.push_back(0);
    setp(begin(), end());

  } else {
    *pptr() = 0;
  }

  std::streamsize n = pptr() - pbase();
  assert(std::numeric_limits<int>::min() < -n);
  pbump((int)-n);

  registry::get_instance().log(mStreamBase, begin());

  assert(n < std::numeric_limits<int>::max());
  return (int)n;
}

streambuf::int_type
streambuf::
overflow(int_type c)
{
  std::streamsize offset = pptr() - pbase();
  mBuffer.push_back(c);
  setp(begin(), end());
  pbump((int)(offset + 1));

  return c;
}

} }

#undef INITIAL_BUFFER_SIZE

