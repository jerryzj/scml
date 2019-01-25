/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_PUT_CALLBACK_H
#define SCML2_BITFIELD_PUT_CALLBACK_H

#include <scml2/memory_put_callback_base.h>

#include <scml2_logging/snps_vp_dll.h>

#include <cassert>

//class TestPutCallbacks;

namespace scml2 {

class SNPS_VP_API bitfield_put_callback_base
  : public memory_put_callback_base
{
 public:
  bitfield_put_callback_base(memory_put_callback_owner* owner)
    : memory_put_callback_base(owner, 0, 0)
  {}
};

template<typename DT, typename ADAPTER_TYPE>
class bitfield_put_callback 
  : public bitfield_put_callback_base
{
public:
  bitfield_put_callback(bitfield<DT>* owner,
			const ADAPTER_TYPE& adapter)
    : bitfield_put_callback_base(&owner->get_register())
    , mOwner(owner)
    , mAdapter(adapter)
    , mOldValue()
  {}

  virtual void store_old_value(unsigned long long index) 
  {
    assert(index == 0);
    (void)index;
    mOldValue = extract_bits((DT)(mOwner->get_register()), mOwner->get_size(), mOwner->get_offset());
  }
  virtual void execute(unsigned long long index) 
  {
    assert(index == 0);
    (void)index;
    const DT new_value = extract_bits((DT)(mOwner->get_register()), mOwner->get_size(), mOwner->get_offset());
    if (new_value != mOldValue) {
      mAdapter(new_value);
    }
  }
 private:
  bitfield<DT>* mOwner;
  ADAPTER_TYPE mAdapter;
  DT mOldValue;
};

template<typename DT, typename ADAPTER_TYPE>
bitfield_put_callback<DT, ADAPTER_TYPE>* create_bitfield_put_callback(bitfield<DT>* owner,
								      const ADAPTER_TYPE& adapter)
{
  return new bitfield_put_callback<DT, ADAPTER_TYPE>(owner, adapter);
}


}

#endif
