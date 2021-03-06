/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_BASE_H
#define SCML2_BITFIELD_BASE_H

#include <scml2/symbolic_names.h>
#include <scml2/utils.h>
#include <scml2_logging/snps_vp_dll.h>

#include <scml2_logging/stream.h>
#include <systemc>
#include <string>
#include <vector>

namespace scml2 {

class bitfield_callback_base;
class bitfield_debug_callback_base;
class bitfield_access_restriction_b;
class streams;

class SNPS_VP_API bitfield_base : public sc_core::sc_object
{
public:
  typedef symbolic_names::SymbolicNames SymbolicNames;

public:
  bitfield_base(const std::string& name,
                unsigned int offset,
                unsigned int size,
                streams& streams);
  virtual ~bitfield_base();

  std::string get_name() const;
  unsigned int get_offset() const;
  unsigned int get_size() const;

  void set_description(const std::string& d);
  const std::string& get_description() const;

  virtual bitfield_callback_base* get_read_callback() const = 0;
  virtual bitfield_callback_base* get_write_callback() const = 0;
  virtual bitfield_debug_callback_base* get_debug_read_callback() const = 0;
  virtual bitfield_debug_callback_base* get_debug_write_callback() const = 0;

  virtual bitfield_access_restriction_b* get_read_restriction() const = 0;
  virtual bitfield_access_restriction_b* get_write_restriction() const = 0;

  streams& get_streams() const;

  bool set_symbolic_name(const std::vector<unsigned char>& value,
                         const std::string& name);
  std::string get_symbolic_name(const std::vector<unsigned char>& value) const;
  unsigned int collect_symbolic_names(SymbolicNames& symbolicNames) const;

protected:
  streams& mStreams;

  const unsigned int mOffset;
  const unsigned int mSize;
  std::string mDescription;

  symbolic_names mSymbolicNames;
};

inline
std::string
bitfield_base::
get_name() const
{
  return name();
}

inline
unsigned int
bitfield_base::
get_offset() const
{
  return mOffset;
}

inline
unsigned int
bitfield_base::
get_size() const
{
  return mSize;
}

inline
streams&
bitfield_base::
get_streams() const
{
  return mStreams;
}

}

#endif
