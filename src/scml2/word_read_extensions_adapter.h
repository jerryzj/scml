/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_WORD_READ_EXTENSIONS_ADAPTER_H
#define SCML2_WORD_READ_EXTENSIONS_ADAPTER_H

#include <scml2/word_adapter_base.h>
#include <scml2/streams.h>
#include <scml2/utils.h>
#include <scml2/tlm2_gp_extensions.h>

#include <scml2_logging/stream.h>
#include <tlm.h>
#include <cassert>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename M, typename C>
class word_read_extensions_adapter : public word_adapter_base<M>
{
public:
  typedef typename M::data_type data_type;
  typedef bool (C::*CallbackType)(data_type& data, sc_core::sc_time&, const tlm2_gp_extensions&);

public:
  word_read_extensions_adapter(C& c, CallbackType cb, const M& mem, bool check_word_access);

  void operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);

private:
  C& mClass;
  CallbackType mCallback;
};

template <typename M, typename C>
inline
word_read_extensions_adapter<M, C>
create_word_read_adapter(C& c,
                         typename word_read_extensions_adapter<M, C>::CallbackType cb,
                         bool check_word_access, const M& mem)
{
  return word_read_extensions_adapter<M, C>(c, cb, mem, check_word_access);
}

template <typename M, typename C>
inline
word_read_extensions_adapter<M, C>::
word_read_extensions_adapter(C& c, CallbackType cb, const M& mem, bool check_word_access) :
  word_adapter_base<M>(sizeOf<data_type>(), &mem, check_word_access),
  mClass(c),
  mCallback(cb)
{
  if (mem.get_size() > 1) {
    SCML2_LOG_ERROR(&mem, mem.get_streams().error(), CONFIGURATION_ERROR)
      << "word_read_extensions_adapter only supported for memory objects of size 1, "
      << "memory '" << mem.get_name() << "' has size " << mem.get_size()
      << std::endl;
  }
}

template <typename M, typename C>
inline
void
word_read_extensions_adapter<M, C>::
operator()(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
  assert(trans.is_read());

  if (this->checkAlignedWordAccesses(trans)) {
    tlm2_gp_extensions extensions(trans);
    for (unsigned int i = 0; i < trans.get_data_length(); i += this->mWordSize) {
      data_type data;
      if (!(mClass.*mCallback)(data, t, extensions)) {
        trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        return;
      }
      this->update_read_data(data, trans, i);
    }
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
  }
}

}

#endif
