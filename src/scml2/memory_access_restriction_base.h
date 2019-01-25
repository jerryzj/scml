/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_ACCESS_RESTRICTION_BASE_H
#define SCML2_MEMORY_ACCESS_RESTRICTION_BASE_H

#include <scml2/resource.h>
#include <scml2/types.h>

#include <string>

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

namespace {
  template<typename DT, template <typename> class MEM_OBJECT>
  void store_into_mem_obj(DT in_data, DT disabled_bit_enables, MEM_OBJECT<DT>& mem_obj)
  {
    mem_obj = ((DT)(mem_obj) & ~disabled_bit_enables) | (in_data & disabled_bit_enables);
  }

  template<typename DT>
  DT get_disabled_bit_enables(DT bit_enables, DT mask)
  {
    return bit_enables & ~mask;
  }

  template<typename DT>
  DT disable_bit_enables(DT& bit_enables, DT mask)
  {
    DT disabled_bit_enables = get_disabled_bit_enables(bit_enables, mask);
    bit_enables &= mask;
    return disabled_bit_enables;
  }
}

template<typename DT>
access_restriction_result
restrict_some(DT& out_data, DT& out_bit_enables, DT in_data, DT in_data_mask)
{
  DT disabled_bit_enables = disable_bit_enables(out_bit_enables, ~in_data_mask);
  out_data = (out_data & ~disabled_bit_enables) | (in_data & disabled_bit_enables);
  return RESTRICT_NO_ERROR;
}

template<typename DT, template <typename> class MEM_OBJECT>
access_restriction_result
restrict_some_and_store(DT& out_data, DT& out_bit_enables, DT in_data, DT in_data_mask, MEM_OBJECT<DT>& mem_obj)
{
  store_into_mem_obj(in_data, get_disabled_bit_enables(out_bit_enables, ~in_data_mask), mem_obj);
  return restrict_some(out_data, out_bit_enables, in_data, in_data_mask);
}

template<typename DT>
access_restriction_result
restrict_all(DT& data, DT& bit_enables, DT restrict_value = DT())
{
  return restrict_some(data, bit_enables, restrict_value, ~DT());
}

template<typename DT, template <typename> class MEM_OBJECT>
access_restriction_result
restrict_all_and_store(DT& data, DT& bit_enables, DT restrict_value, MEM_OBJECT<DT>& mem_obj)
{
  store_into_mem_obj(restrict_value, get_disabled_bit_enables(bit_enables, DT()), mem_obj);
  return restrict_all(data, bit_enables, restrict_value);
}

template<typename DT>
access_restriction_result
restrict_all(tlm::tlm_generic_payload& trans, DT restrict_value = DT())
{
  assert(trans.get_byte_enable_ptr() != 0);
  assert(trans.get_byte_enable_length() == trans.get_data_length());
  unsigned offset = trans.get_address()%sizeOf<DT>();
  for (unsigned i = 0; i != trans.get_data_length(); ++i) {
    unsigned char& be = trans.get_byte_enable_ptr()[i];
    unsigned char& data = trans.get_data_ptr()[i];
    unsigned char restrict_value_bits = extract_bits(restrict_value, 8, 8*offset);
    data = (restrict_value_bits & be) | (data & ~be);
    be = 0x0;
    offset = (offset+1)%sizeOf<DT>();
  }
  return RESTRICT_NO_ERROR;
}

class memory_access_restriction_base : public resource
{
public:
  memory_access_restriction_base(const std::string& name = "<anonymous>");
  virtual ~memory_access_restriction_base();

  const std::string& get_name() const;

  virtual access_restriction_result restrict(tlm::tlm_generic_payload& trans) = 0;

  virtual bool has_default_restriction() const;
private:
  const std::string mName;
};

inline
memory_access_restriction_base::
memory_access_restriction_base(const std::string& name) :
  mName(name)
{
}

inline
memory_access_restriction_base::
~memory_access_restriction_base()
{
}

inline
const std::string&
memory_access_restriction_base::
get_name() const
{
  return mName;
}

inline 
bool
memory_access_restriction_base::
has_default_restriction() const
{
  return false;
}

}

#endif
