/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#include "scml2/memory_access_restriction_base.h"
#include <scml2/restriction_adapter.h>
#include <scml2/word_restriction_adapter.h>
#include "scml2/ignore_restriction_adapter.h"

#include <scml2/reg.h>
#include <type_traits>
#include <cassert>

#include "scml2/only_word_restriction_adapter.h"
#include "scml2/write_once_restriction_adapter.h"


namespace scml2 {

class write_once_state;

template <typename ADAPTER>
inline
memory_access_restriction_base*
create_memory_restriction(ADAPTER adapter, const std::string& name)
{
  return new memory_restriction<ADAPTER, memory_access_restriction_base>(adapter, name);
}

template <typename DT, template <typename> class MEM_OBJECT, typename C>
inline void set_read_restriction(MEM_OBJECT<DT>& mem, C* c, typename restriction_adapter<C>::RestrictionType r, const std::string& rName) {
  assert(c);
  mem.set_read_restriction(create_memory_restriction(create_restriction_adapter(*c, r), rName));
}

template <typename DT, template <typename> class MEM_OBJECT, typename C>
inline void set_write_restriction(MEM_OBJECT<DT>& mem, C* c, typename restriction_adapter<C>::RestrictionType r, const std::string& rName) {
  assert(c);
  mem.set_write_restriction(create_memory_restriction(create_restriction_adapter(*c, r), rName));
}

template <typename DT, template <typename> class MEM_OBJECT, typename C>
inline void set_restriction(MEM_OBJECT<DT>& mem, C* c, typename restriction_adapter<C>::RestrictionType r, const std::string& rName) {
  assert(c);
  mem.set_restriction(create_memory_restriction(create_restriction_adapter(*c, r), rName));
}

template <typename DT, template <typename> class MEM_OBJECT, typename C>
inline void set_read_restriction(MEM_OBJECT<DT>& mem, C* c, typename restriction_adapter_tagged<C>::RestrictionType r, const std::string& rName, int id) {
  assert(c);
  mem.set_read_restriction(create_memory_restriction(create_restriction_adapter_tagged(*c, r, id), rName));
}

template <typename DT, template <typename> class MEM_OBJECT, typename C>
inline void set_write_restriction(MEM_OBJECT<DT>& mem, C* c, typename restriction_adapter_tagged<C>::RestrictionType r, const std::string& rName, int id) {
  assert(c);
  mem.set_write_restriction(create_memory_restriction(create_restriction_adapter_tagged(*c, r, id), rName));
}

template <typename DT, template <typename> class MEM_OBJECT, typename C>
inline void set_restriction(MEM_OBJECT<DT>& mem, C* c, typename restriction_adapter_tagged<C>::RestrictionType r, const std::string& rName, int id) {
  assert(c);
  mem.set_restriction(create_memory_restriction(create_restriction_adapter_tagged(*c, r, id), rName));
}

template <typename DT, template <typename> class MEM_OBJECT, typename C>
inline void set_read_restriction(MEM_OBJECT<DT>& mem, C* c, typename word_restriction_adapter<MEM_OBJECT<DT>, C>::RestrictionType r, const std::string& rName) {
  static_assert(std::is_base_of<reg_base,MEM_OBJECT<DT> >::value, "restriction callbacks with DT arguments are only supported for scml2::reg<DT> memory objects");
  assert(c);
  mem.set_read_restriction(create_memory_restriction(create_word_restriction_adapter(*c, r, mem), rName));
}

template <typename DT, template <typename> class MEM_OBJECT, typename C>
inline void set_write_restriction(MEM_OBJECT<DT>& mem, C* c, typename word_restriction_adapter<MEM_OBJECT<DT>, C>::RestrictionType r, const std::string& rName) {
  static_assert(std::is_base_of<reg_base,MEM_OBJECT<DT> >::value, "restriction callbacks with DT arguments are only supported for scml2::reg<DT> memory objects");
  assert(c);
  mem.set_write_restriction(create_memory_restriction(create_word_restriction_adapter(*c, r, mem), rName));
}

template <typename DT, template <typename> class MEM_OBJECT, typename C>
inline void set_restriction(MEM_OBJECT<DT>& mem, C* c, typename word_restriction_adapter<MEM_OBJECT<DT>, C>::RestrictionType r, const std::string& rName) {
  static_assert(std::is_base_of<reg_base,MEM_OBJECT<DT> >::value, "restriction callbacks with DT arguments are only supported for scml2::reg<DT> memory objects");
  assert(c);
  mem.set_restriction(create_memory_restriction(create_word_restriction_adapter(*c, r, mem), rName));
}

template <typename DT, template <typename> class MEM_OBJECT, typename C>
inline void set_read_restriction(MEM_OBJECT<DT>& mem, C* c, typename word_restriction_adapter_tagged<MEM_OBJECT<DT>, C>::RestrictionType r, const std::string& rName, int id) {
  static_assert(std::is_base_of<reg_base,MEM_OBJECT<DT> >::value, "restriction callbacks with DT arguments are only supported for scml2::reg<DT> memory objects");
  assert(c);
  mem.set_read_restriction(create_memory_restriction(create_word_restriction_adapter_tagged(*c, r, id, mem), rName));
}

template <typename DT, template <typename> class MEM_OBJECT, typename C>
inline void set_write_restriction(MEM_OBJECT<DT>& mem, C* c, typename word_restriction_adapter_tagged<MEM_OBJECT<DT>, C>::RestrictionType r, const std::string& rName, int id) {
  static_assert(std::is_base_of<reg_base,MEM_OBJECT<DT> >::value, "restriction callbacks with DT arguments are only supported for scml2::reg<DT> memory objects");
  assert(c);
  mem.set_write_restriction(create_memory_restriction(create_word_restriction_adapter_tagged(*c, r, id, mem), rName));
}

template <typename DT, template <typename> class MEM_OBJECT, typename C>
inline void set_restriction(MEM_OBJECT<DT>& mem, C* c, typename word_restriction_adapter_tagged<MEM_OBJECT<DT>, C>::RestrictionType r, const std::string& rName, int id) {
  static_assert(std::is_base_of<reg_base,MEM_OBJECT<DT> >::value, "restriction callbacks with DT arguments are only supported for scml2::reg<DT> memory objects");
  assert(c);
  mem.set_restriction(create_memory_restriction(create_word_restriction_adapter_tagged(*c, r, id, mem), rName));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline void set_read_ignore_restriction(MEM_OBJECT<DT>& mem, DT value = DT()) {
  mem.set_read_restriction(create_memory_restriction(create_ignore_restriction_adapter<DT>(value), "ignore read restriction"));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline void set_write_ignore_restriction(MEM_OBJECT<DT>& mem, DT value = DT()) {
  mem.set_write_restriction(create_memory_restriction(create_ignore_restriction_adapter<DT>(value), "ignore write restriction"));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline void set_ignore_restriction(MEM_OBJECT<DT>& mem, DT value = DT()) {
  mem.set_restriction(create_memory_restriction(create_ignore_restriction_adapter<DT>(value), "ignore access restriction"));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline void set_read_error_restriction(MEM_OBJECT<DT>& mem) {
  mem.set_read_restriction(create_memory_restriction(create_error_restriction_adapter<DT>(), "error read restriction"));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline void set_write_error_restriction(MEM_OBJECT<DT>& mem) {
  mem.set_write_restriction(create_memory_restriction(create_error_restriction_adapter<DT>(), "error write restriction"));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline void set_error_restriction(MEM_OBJECT<DT>& mem) {
  mem.set_restriction(create_memory_restriction(create_error_restriction_adapter<DT>(), "error access restriction"));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline void set_read_word_error_restriction(MEM_OBJECT<DT>& mem) {
  mem.set_read_restriction(create_memory_restriction(create_only_word_restriction_adapter<DT>(true), "word read error restriction"));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline void set_write_word_error_restriction(MEM_OBJECT<DT>& mem) {
  mem.set_write_restriction(create_memory_restriction(create_only_word_restriction_adapter<DT>(true), "word write error restriction"));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline void set_word_error_restriction(MEM_OBJECT<DT>& mem) {
  mem.set_restriction(create_memory_restriction(create_only_word_restriction_adapter<DT>(true), "word access error restriction"));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline void set_read_word_ignore_restriction(MEM_OBJECT<DT>& mem, DT value = DT()) {
  mem.set_read_restriction(create_memory_restriction(create_only_word_restriction_adapter<DT>(false, value), "word read ignore restriction"));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline void set_write_word_ignore_restriction(MEM_OBJECT<DT>& mem, DT value = DT()) {
  mem.set_write_restriction(create_memory_restriction(create_only_word_restriction_adapter<DT>(false, value), "word write ignore restriction"));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline void set_word_ignore_restriction(MEM_OBJECT<DT>& mem, DT value = DT()) {
  mem.set_restriction(create_memory_restriction(create_only_word_restriction_adapter<DT>(false, value), "word access ignore restriction"));
}

template <typename DT, template <typename> class MEM_OBJECT>
inline write_once_state* set_write_once_error_restriction(MEM_OBJECT<DT>& mem) {
  write_once_state* result;
  mem.set_write_restriction(create_memory_restriction(create_write_once_restriction_adapter<DT>(true, DT(), &result), "write once error restriction"));
  return result;
}

template <typename DT, template <typename> class MEM_OBJECT>
inline write_once_state* set_write_once_ignore_restriction(MEM_OBJECT<DT>& mem, DT value = DT()) {
  write_once_state* result;
  mem.set_write_restriction(create_memory_restriction(create_write_once_restriction_adapter<DT>(false, value, &result), "write once ignore restriction"));
  return result;
}

}
