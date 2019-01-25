/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#include <scml2/memory_put_callback.h>
#include <scml2/memory_put_callback_adapter.h>
#include <scml2/memory_tagged_put_callback_adapter.h>

#include <scml2/bitfield_put_callback.h>
#include <scml2/bitfield_put_callback_adapter.h>

namespace scml2 {

template<typename DT, template<typename> class MEM_OBJECT>
inline void add_put_callback(MEM_OBJECT<DT>& mem, memory_put_callback_base* cb)
{
  cb->ref();
  mem.register_content_observer(cb);
}

template<typename DT>
inline void add_put_callback(bitfield<DT>& bf, bitfield_put_callback_base* cb)
{
  cb->ref();
  bf.get_register().register_content_observer(cb);
}


template<typename DT, template<typename> class MEM_OBJECT, typename C>
inline memory_put_callback_base* add_put_callback(MEM_OBJECT<DT>& mem, C* c, typename memory_put_callback_adapter<C, DT>::CallbackType cb, const std::string& /*cbName*/) {
  assert(cb);
  memory_put_callback_base* result = create_memory_put_callback(&mem, create_memory_put_callback_adapter<C, DT>(*c, cb), 0, mem.get_size()-1);
  add_put_callback(mem, result);
  return result;
}

template<typename DT, template<typename> class MEM_OBJECT, typename C>
inline memory_put_callback_base* add_put_callback(MEM_OBJECT<DT>& mem, C* c, typename memory_tagged_put_callback_adapter<C, DT>::CallbackType cb, const std::string& /*cbName*/, int id) {
  assert(cb);
  memory_put_callback_base* result = create_memory_put_callback(&mem, create_memory_tagged_put_callback_adapter<C, DT>(*c, cb, id), 0, mem.get_size()-1);
  add_put_callback(mem, result);
  return result;
}

template<typename DT, typename C>
inline bitfield_put_callback_base* add_put_callback(bitfield<DT>& bf, C* c, typename bitfield_put_callback_adapter<C, DT>::CallbackType cb, const std::string& /*cbName*/) {
  assert(cb);
  bitfield_put_callback_base* result = create_bitfield_put_callback(&bf, create_bitfield_put_callback_adapter<C, DT>(*c, cb));
  add_put_callback(bf, result);
  return result;
}

template<typename DT, typename C>
inline bitfield_put_callback_base* add_put_callback(bitfield<DT>& bf, C* c, typename bitfield_tagged_put_callback_adapter<C, DT>::CallbackType cb, const std::string& /*cbName*/, int id) {
  assert(cb);
  bitfield_put_callback_base* result = create_bitfield_put_callback(&bf, create_bitfield_tagged_put_callback_adapter<C, DT>(*c, cb, id));
  add_put_callback(bf, result);
  return result;
}

template<typename DT, template<typename> class MEM_OBJECT>
inline bool remove_put_callback(MEM_OBJECT<DT>& mem, memory_put_callback_base* cb) {
  assert(cb);
  bool result = mem.unregister_content_observer(cb);
  cb->unref();
  return result;
}

template<typename DT>
inline bool remove_put_callback(bitfield<DT>& bf, bitfield_put_callback_base* cb) {
  assert(cb);
  bool result = bf.get_register().unregister_content_observer(cb);
  cb->unref();
  return result;
}

}
