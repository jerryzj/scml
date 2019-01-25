/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_WRITE_EXTENSIONS_ADAPTER_H
#define SCML2_WRITE_EXTENSIONS_ADAPTER_H

#include <scml2/streams.h>
#include <scml2_logging/stream.h>
#include <scml2/tlm2_gp_extensions.h>
#include <cassert>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename M, typename C>
class write_extensions_adapter

{
public:
  typedef typename M::data_type data_type;
  typedef bool (C::*CallbackType)(const data_type& data,
                                  const data_type& byteEnables, 
                                  sc_core::sc_time&,
                                  const tlm2_gp_extensions&);

public:
  write_extensions_adapter(C& c, CallbackType cb, const M& mem);

  bool operator()(const data_type& data,
                  const data_type& byteEnables,
                  sc_core::sc_time& t, 
                  const tlm2_gp_extensions& extensions);

private:
  C& mClass;
  CallbackType mCallback;
};

template <typename M, typename C>
inline
write_extensions_adapter<M, C>
create_write_adapter(C& c,
                     typename write_extensions_adapter<M, C>::CallbackType cb,
                     const M& mem)
{
  return write_extensions_adapter<M, C>(c, cb, mem);
}

template <typename M, typename C>
inline
write_extensions_adapter<M, C>::
write_extensions_adapter(C& c, CallbackType cb, const M& mem) :
  mClass(c),
  mCallback(cb)
{
  if (mem.get_size() > 1) {
    SCML2_LOG_ERROR(&mem, mem.get_streams().error(), CONFIGURATION_ERROR)
      << "write_extensions_adapter only supported for memory objects of size 1, "
      << "memory '" << mem.get_name() << "' has size " << mem.get_size()
      << std::endl;
  }
}

template <typename M, typename C>
inline
bool
write_extensions_adapter<M, C>::
operator()(const data_type& data,
           const data_type& byteEnables,
           sc_core::sc_time& t, 
           const tlm2_gp_extensions& extensions)
{
  assert(mCallback);
  return (mClass.*mCallback)(data, byteEnables, t, extensions);
}

}

#endif
