/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_WRITE_ADAPTER_TAGGED_H
#define SCML2_WRITE_ADAPTER_TAGGED_H

#include <scml2/streams.h>
#include <scml2_logging/stream.h>
#include <scml2/tlm2_gp_extensions.h>
#include <cassert>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename M, typename C>
class write_adapter_tagged
{
public:
  typedef typename M::data_type data_type;
  typedef bool (C::*CallbackType)(const data_type& data,
                                  const data_type& byteEnables,
                                  sc_core::sc_time&,
                                  int id);

public:
  write_adapter_tagged(C& c, CallbackType cb, int id, const M& mem);

  bool operator()(data_type& data,
                  const data_type& byteEnables,
                  sc_core::sc_time& t, 
                  const tlm2_gp_extensions&);

private:
  C& mClass;
  CallbackType mCallback;
  int mId;
};

template <typename M, typename C>
inline
write_adapter_tagged<M, C>
create_write_adapter(C& c,
                     typename write_adapter_tagged<M, C>::CallbackType cb,
                     int id,
                     const M& mem)
{
  return write_adapter_tagged<M, C>(c, cb, id, mem);
}

template <typename M, typename C>
inline
write_adapter_tagged<M, C>::
write_adapter_tagged(C& c, CallbackType cb, int id, const M& mem) :
  mClass(c),
  mCallback(cb),
  mId(id)
{
  if (mem.get_size() > 1) {
    SCML2_LOG_ERROR(&mem, mem.get_streams().error(), CONFIGURATION_ERROR)
      << "write_adapter_tagged only supported for memory objects of size 1, "
      << "memory '" << mem.get_name() << "' has size " << mem.get_size()
      << std::endl;
  }
}

template <typename M, typename C>
inline
bool
write_adapter_tagged<M, C>::
operator()(data_type& data,
           const data_type& byteEnables,
           sc_core::sc_time& t, 
           const tlm2_gp_extensions&)
{
  assert(mCallback);
  return (mClass.*mCallback)(data, byteEnables, t, mId);
}

}

#endif
