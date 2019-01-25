/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#include <scml2/memory_alias.h>

#include <scml2/memory.h>
#include <scml2/memory_default_callback.h>
#include <scml2/memory_default_debug_callback.h>
#include <scml2/memory_default_access_restriction.h>


namespace scml2 {

memory_alias_base::memory_alias_base(toplevel_memory_base& memory,
				     const std::string& name,
				     memory_base* parent,
				     unsigned long long offset,
				     unsigned long long size,
				     unsigned int width)
  : memory_base(name, parent, offset, size, width)
  , mToplevel(memory)
{
}
  
memory_alias_base::~memory_alias_base()
{
}

template <typename DT>
void 
memory_alias<DT>::
check_size(unsigned long long size) const
{
  if ((size*mToplevel.get_width()) % sizeOf<DT>()) {
    SCML2_LOG_ERROR(this, get_streams().error(), CONFIGURATION_ERROR)
      << "The alias '" << get_name()
      << "' has an invalid size argument: '" << size << "' (not a multiple of its width multiplier with regards to its toplevel '" << sizeOf<DT>()/mToplevel.get_width() << "'"
      << std::endl;
  }
}

template <typename DT>
memory_alias<DT>::
memory_alias(const std::string& name,
             toplevel_memory_base& parent,
             unsigned long long offset,
             unsigned long long size) :
  memory_alias_base(parent, name, &parent, offset, (size*parent.get_width())/sizeOf<DT>(), sizeOf<DT>()),
  mFastAccess()
{
  setup();
  check_size(size);
}

template <typename DT>
memory_alias<DT>::
memory_alias(const std::string& name,
             memory_alias_base& parent,
             unsigned long long offset,
             unsigned long long size) :
  memory_alias_base(parent.mToplevel, name, &parent, parent.get_offset() + offset, (size*parent.mToplevel.get_width())/sizeOf<DT>(), sizeOf<DT>()),
  mFastAccess()
{
  setup();
  check_size(size);
}

template <typename DT>
memory_alias<DT>::
~memory_alias()
{
}

template <typename DT>
void
memory_alias<DT>::
initialize(const DT& value)
{
  for (unsigned int i = 0; i < get_size(); ++i) {
    put_debug(i, value);
  }
}

template <typename DT>
bool
memory_alias<DT>::
set_symbolic_name(const DT& value, const std::string& name)
{
  std::vector<unsigned char> tmp(sizeOf<DT>());
  to_char_array(value, &tmp[0], sizeOf<DT>(), 0);
  return set_symbolic_name(tmp, name);
}

template <typename DT>
std::string
memory_alias<DT>::
get_symbolic_name(const DT& value) const
{
  std::vector<unsigned char> tmp(sizeOf<DT>());
  to_char_array(value, &tmp[0], sizeOf<DT>(), 0);
  return get_symbolic_name(tmp);
}

template <typename DT>
void
memory_alias<DT>::
setup()
{
  init_streams(mToplevel.get_streams());

  init_default_callback(new memory_default_callback<DT>(mFastAccess, mToplevel));
  init_default_debug_callback(new memory_default_debug_callback<DT>(mFastAccess));
  init_default_restriction(new memory_default_access_restriction<DT>());

  update_fast_access();
}

template <typename DT>
void
memory_alias<DT>::
invalidate_dmi()
{
  mToplevel.invalidate_dmi();
}

template <typename DT>
void
memory_alias<DT>::
update_fast_access()
{
  mFastAccess.update(mToplevel.template create_slow_access<DT>(get_offset()));
}

template <typename DT>
unsigned int
memory_alias<DT>::
read(unsigned long long address,
     unsigned char* data,
     unsigned int size)
{
  return mToplevel.read(address + mToplevel.get_width()*get_offset(),
                        data,
                        size);
}

template <typename DT>
unsigned int
memory_alias<DT>::
write(unsigned long long address,
      const unsigned char* data,
      unsigned int size)
{
  return mToplevel.write(address + mToplevel.get_width()*get_offset(),
                         data,
                         size);
}

template <typename DT>
bool
memory_alias<DT>::
register_content_observer(memory_content_observer* o)
{
  return mToplevel.register_content_observer(o, get_offset());
}

template <typename DT>
bool
memory_alias<DT>::
unregister_all_content_observers()
{
  return mToplevel.unregister_all_content_observers();
}

template <typename DT>
bool
memory_alias<DT>::
unregister_content_observer(memory_content_observer* o)
{
  return mToplevel.unregister_content_observer(o);
}


template <typename DT>
const char*
memory_alias<DT>::
kind() const
{
  return "scml2::memory_alias";
}

#ifdef _WIN32
#define SCML2_INSTANTIATE_TEMPLATE(type) template class SNPS_VP_API memory_alias<type >;
#else
#define SCML2_INSTANTIATE_TEMPLATE(type) template class memory_alias<type >;
#endif
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}
