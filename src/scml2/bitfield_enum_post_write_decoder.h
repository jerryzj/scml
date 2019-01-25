/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_BITFIELD_ENUM_POST_WRITE_DECODER_H
#define SCML2_BITFIELD_ENUM_POST_WRITE_DECODER_H

#include <scml2/bitfield_post_write_callback_base.h>

#include <tlm.h>
#include <string>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT>
class bitfield_enum_post_write_decoder :
  public bitfield_post_write_callback_base<DT>
{
public:
  bitfield_enum_post_write_decoder(const std::string& name);

  virtual bool post_write(const DT& old_value, const DT& value, sc_core::sc_time& t);
  virtual bool has_never_syncing_behavior() const;
  virtual bool has_default_behavior() const;
  void register_decode_cb(DT enumval, bitfield_post_write_callback_base<DT>* cb);
  void register_default_cb(bitfield_post_write_callback_base<DT>* cb);
 private:
  typedef std::map<DT, bitfield_post_write_callback_base<DT>* >  t_cbmap;
  bitfield_post_write_callback_base<DT>* m_default_cb;
  t_cbmap m_cbs;
};

template <typename DT>
inline
bitfield_enum_post_write_decoder<DT>::
bitfield_enum_post_write_decoder(const std::string& name) :
    bitfield_post_write_callback_base<DT>(name),
    m_default_cb(0),
    m_cbs()
{
}


template <typename DT>
inline
bool
bitfield_enum_post_write_decoder<DT>::
has_default_behavior() const
{
  return true;
}

template <typename DT>
inline
void
bitfield_enum_post_write_decoder<DT>::
register_decode_cb(DT enumval, bitfield_post_write_callback_base<DT>* cb)
{
  assert(cb);
  m_cbs[enumval] = cb;
}

template <typename DT>
inline
void
bitfield_enum_post_write_decoder<DT>::
register_default_cb(bitfield_post_write_callback_base<DT>* cb)
{
  assert(cb);
  m_default_cb = cb;
}



template <typename DT>
inline
bool
bitfield_enum_post_write_decoder<DT>::
post_write(const DT& old_value, const DT& value, sc_core::sc_time& t)
{
  auto i = m_cbs.find(value);
  if (i != m_cbs.end()){
    bitfield_post_write_callback_base<DT>* p = i->second;
    if (p != 0 ) {
        return p->post_write(old_value, value, t);
    } 
    return false;
  }
  if (m_default_cb) {
    return m_default_cb->post_write(old_value, value,t);
  }
  return true;
}


template <typename DT>
inline
bool
bitfield_enum_post_write_decoder<DT>::
has_never_syncing_behavior() const
{
  return true;
}


}

#endif
