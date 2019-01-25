/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/memory.h>

#include <scml2/memory_storage_base.h>
#include <scml2/memory_storage_creator.h>
#include <scml2/memory_default_callback.h>
#include <scml2/memory_default_debug_callback.h>
#include <scml2/memory_default_access_restriction.h>
#include <scml2/streams.h>

#include <scml2_logging/stream.h>
#include <algorithm>
#include <cassert>


namespace scml2 {

template <typename DT>
memory<DT>::
memory(const std::string& name,
       unsigned long long size) :
  toplevel_memory_base(name, size, sizeOf<DT>()),
  mFastAccess()
{
  setup(memory_storage_creator::get_instance().create_storage(*this), false);
}

template <typename DT>
memory<DT>::
memory(const std::string& name,
       unsigned long long size,
       memory_storage_base* storage,
       bool delStorageOnDestruction) :
  toplevel_memory_base(name, size, sizeOf<DT>()),
  mFastAccess()
{
  setup(storage, delStorageOnDestruction);
}

template <typename DT>
memory<DT>::
~memory()
{
}

template <typename DT>
void
memory<DT>::
initialize(const DT& value)
{
  for (unsigned int i = 0; i < get_size(); ++i) {
    put_debug(i, value);
  }
}

template <typename DT>
bool
memory<DT>::
set_symbolic_name(const DT& value, const std::string& name)
{
  std::vector<unsigned char> tmp(sizeOf<DT>());
  to_char_array(value, &tmp[0], sizeOf<DT>(), 0);
  return set_symbolic_name(tmp, name);
}

template <typename DT>
std::string
memory<DT>::
get_symbolic_name(const DT& value)
{
  std::vector<unsigned char> tmp(sizeOf<DT>());
  to_char_array(value, &tmp[0], sizeOf<DT>(), 0);
  return get_symbolic_name(tmp);
}

template <typename DT>
void
memory<DT>::
setup(memory_storage_base* storage, bool delStorageOnDestruction)
{

  set_storage(storage, delStorageOnDestruction);
  init_default_callback(new memory_default_callback<DT>(mFastAccess, *this));
  init_default_debug_callback(new memory_default_debug_callback<DT>(mFastAccess));
  init_default_restriction(new memory_default_access_restriction<DT>());

  update_fast_access();
}


template <typename DT>
void
memory<DT>::
update_fast_access()
{
  mFastAccess.update(create_slow_access<DT>(0));
}

#ifdef _WIN32
#define SCML2_INSTANTIATE_TEMPLATE(type) template class SNPS_VP_API memory<type >;
#else
#define SCML2_INSTANTIATE_TEMPLATE(type) template class memory<type >;
#endif
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}
