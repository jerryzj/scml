/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/bitfield_base.h>
#include <scml2/streams.h>
#include <scml2/utils.h>

#include <scml2_logging/severity.h>

namespace scml2 {

bitfield_base::
bitfield_base(const std::string& name,
              unsigned int offset,
              unsigned int size,
              streams& streams) :
  sc_core::sc_object(name.c_str()),
  mStreams(streams),
  mOffset(offset),
  mSize(size)
{
}

bitfield_base::
~bitfield_base()
{
}

void
bitfield_base::
set_description(const std::string& d)
{
  mDescription = d;
}

const std::string&
bitfield_base::
get_description() const
{
  return mDescription;
}

bool
bitfield_base::
set_symbolic_name(const std::vector<unsigned char>& value, const std::string& name)
{
  return mSymbolicNames.set_symbolic_name(value, name);
}

std::string
bitfield_base::
get_symbolic_name(const std::vector<unsigned char>& value) const {
  return mSymbolicNames.get_symbolic_name(value);
}

unsigned int
bitfield_base::
collect_symbolic_names(SymbolicNames& symbolicNames) const
{
  return mSymbolicNames.collect_symbolic_names(symbolicNames);
}

}
