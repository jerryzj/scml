/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_PUT_CALLBACK_H
#define SCML2_MEMORY_PUT_CALLBACK_H

#include <scml2/memory_put_callback_base.h>

#include <scml2/types.h>

#include <scml2_logging/snps_vp_dll.h>

class TestPutCallbacks;

namespace scml2 {

  namespace {
    template<typename C, typename DT>
    struct index_helper {
      static DT get(C& c, unsigned long long index) {
	return c[index];
      }
    };

    template<typename DT>
    struct index_helper<reg<DT>, DT> {
      static DT get(reg<DT>& reg, unsigned long long index) {
	assert(index == 0);
	(void)index;
	return (DT)reg;
      }
    };
  }

template<typename DT, template<typename> class MEM_OBJECT, typename ADAPTER_TYPE>
class memory_put_callback 
  : public memory_put_callback_base
{
public:
  memory_put_callback(MEM_OBJECT<DT>* owner,
		      const ADAPTER_TYPE& adapter,
		      unsigned long long startIndex,
		      unsigned long long endIndex)
    : memory_put_callback_base(owner, startIndex, endIndex)
    , mOwner(owner)
    , mAdapter(adapter)
    , mOldValue()
  {}

  virtual void store_old_value(unsigned long long index) 
  {
    mOldValue = index_helper<MEM_OBJECT<DT>, DT>::get(*mOwner, index);
  }
  virtual void execute(unsigned long long index) 
  {
    const DT new_value = index_helper<MEM_OBJECT<DT>, DT>::get(*mOwner, index);
    if (new_value != mOldValue) {
      mAdapter(new_value, index);
    }
  }
 protected:
  friend class ::TestPutCallbacks;
  const ADAPTER_TYPE& get_adapter() { return mAdapter; }
 private:

  MEM_OBJECT<DT>* mOwner;
  ADAPTER_TYPE mAdapter;
  DT mOldValue;
};

template<typename DT, template<typename> class MEM_OBJECT, typename ADAPTER_TYPE>
memory_put_callback<DT, MEM_OBJECT, ADAPTER_TYPE>* create_memory_put_callback(MEM_OBJECT<DT>* owner,
										  const ADAPTER_TYPE& adapter,
										  unsigned long long startIndex,
										  unsigned long long endIndex)
{
  return new memory_put_callback<DT, MEM_OBJECT, ADAPTER_TYPE>(owner, adapter, startIndex, endIndex);
}


}

#endif
