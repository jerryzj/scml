/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#include "scml2/bitfield_access_restriction_base.h"
#include <scml2/restriction_adapter.h>
#include "scml2/ignore_restriction_adapter.h"

namespace scml2 {

class write_once_state;

template <typename DT, typename ADAPTER>
inline
bitfield_access_restriction_base<DT>*
create_bitfield_restriction(ADAPTER adapter, const std::string& name)
{
  return new bitfield_restriction<ADAPTER, bitfield_access_restriction_base<DT> >(adapter, name);
}

template <typename DT, typename C>
inline void set_read_restriction(bitfield<DT>& bf, C* c, typename bitfield_restriction_adapter<DT, C>::RestrictionType r, const std::string& rName) {
  assert(c);
  bf.set_read_restriction(create_bitfield_restriction<DT>(create_bitfield_restriction_adapter<DT>(*c, r), rName));
}

template <typename DT, typename C>
inline void set_write_restriction(bitfield<DT>& bf, C* c, typename bitfield_restriction_adapter<DT, C>::RestrictionType r, const std::string& rName) {
  assert(c);
  bf.set_write_restriction(create_bitfield_restriction<DT>(create_bitfield_restriction_adapter<DT>(*c, r), rName));
}

template <typename DT, typename C>
inline void set_restriction(bitfield<DT>& bf, C* c, typename bitfield_restriction_adapter<DT, C>::RestrictionType r, const std::string& rName) {
  assert(c);
  bf.set_restriction(create_bitfield_restriction<DT>(create_bitfield_restriction_adapter<DT>(*c, r), rName));
}

template <typename DT, typename C>
inline void set_read_restriction(bitfield<DT>& bf, C* c, typename bitfield_restriction_adapter_tagged<DT, C>::RestrictionType r, const std::string& rName, int id) {
  assert(c);
  bf.set_read_restriction(create_bitfield_restriction<DT>(create_bitfield_restriction_adapter_tagged<DT>(*c, r, id), rName));
}

template <typename DT, typename C>
inline void set_write_restriction(bitfield<DT>& bf, C* c, typename bitfield_restriction_adapter_tagged<DT, C>::RestrictionType r, const std::string& rName, int id) {
  assert(c);
  bf.set_write_restriction(create_bitfield_restriction<DT>(create_bitfield_restriction_adapter_tagged<DT>(*c, r, id), rName));
}

template <typename DT, typename C>
inline void set_restriction(bitfield<DT>& bf, C* c, typename bitfield_restriction_adapter_tagged<DT, C>::RestrictionType r, const std::string& rName, int id) {
  assert(c);
  bf.set_restriction(create_bitfield_restriction<DT>(create_bitfield_restriction_adapter_tagged<DT>(*c, r, id), rName));
}

template <typename DT>
inline void set_read_ignore_restriction(bitfield<DT>& bf, DT value = DT()) {
  bf.set_read_restriction(create_bitfield_restriction<DT>(create_ignore_restriction_adapter<DT>(value), "ignore read restriction"));
}

template <typename DT>
inline void set_write_ignore_restriction(bitfield<DT>& bf, DT value = DT()) {
  bf.set_write_restriction(create_bitfield_restriction<DT>(create_ignore_restriction_adapter<DT>(value), "ignore write restriction"));
}

template <typename DT>
inline void set_ignore_restriction(bitfield<DT>& bf, DT value = DT()) {
  bf.set_restriction(create_bitfield_restriction<DT>(create_ignore_restriction_adapter<DT>(value), "ignore access restriction"));
}

template <typename DT>
inline void set_read_error_restriction(bitfield<DT>& bf) {
  bf.set_read_restriction(create_bitfield_restriction<DT>(create_error_restriction_adapter<DT>(), "error read restriction"));
}

template <typename DT>
inline void set_write_error_restriction(bitfield<DT>& bf) {
  bf.set_write_restriction(create_bitfield_restriction<DT>(create_error_restriction_adapter<DT>(), "error write restriction"));
}

template <typename DT>
inline void set_error_restriction(bitfield<DT>& bf) {
  bf.set_restriction(create_bitfield_restriction<DT>(create_error_restriction_adapter<DT>(), "error access restriction"));
}

template <typename DT>
inline write_once_state* set_write_once_error_restriction(bitfield<DT>& bf) {
  write_once_state* result;
  bf.set_restriction(create_bitfield_restriction<DT>(create_write_once_restriction_adapter<DT>(true, DT(), &result), "write once error restriction"));
  return result;
}

template <typename DT>
inline write_once_state* set_write_once_ignore_restriction(bitfield<DT>& bf, DT value = DT()) {
  write_once_state* result;
  bf.set_restriction(create_bitfield_restriction<DT>(create_write_once_restriction_adapter<DT>(false, value, &result), "write once ignore restriction"));
  return result;
}

}
