/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_LOGGING_STREAMBUF_H
#define SCML2_LOGGING_STREAMBUF_H

#include <scml2_logging/snps_vp_dll.h>
#include <ios>
#include <vector>

namespace scml2 { namespace logging {

class stream_base;

class SNPS_VP_API streambuf : public std::streambuf
{
public:
  typedef std::streambuf::int_type int_type;

public:
  streambuf(const stream_base& streamBase);

  virtual std::streamsize xsputn(const char* p, std::streamsize n);
  virtual int sync();
  virtual int_type overflow(int_type c);

private:
  char* begin();
  char* end();

private:
  std::vector<char> mBuffer;
  const stream_base& mStreamBase;

  // disabled
  streambuf();
  streambuf(const streambuf&);
  streambuf& operator= (const streambuf&);
};

inline
char*
streambuf::
begin()
{
  return &mBuffer.front();
}

inline
char*
streambuf::
end()
{
  return &mBuffer.back() + 1;
}

} }

#endif
