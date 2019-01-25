/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/ 
#ifndef SCML2_MEMORY_DEFAULT_CALLBACK_H
#define SCML2_MEMORY_DEFAULT_CALLBACK_H

#include <scml2/memory_callback_base.h>

#include <scml2/memory_fast_access.h>
#include <scml2/toplevel_memory_base.h>

#include <tlm.h>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT>
class memory_default_callback : public memory_callback_base
{
public:
  memory_default_callback(memory_fast_access<DT>& fastAccess,
                          toplevel_memory_base& mem);

  virtual void execute(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
  virtual bool has_default_behavior() const;
  virtual bool has_never_syncing_behavior() const;

protected:
  memory_fast_access<DT>& mFastAccess;
  toplevel_memory_base& mMemory;
};

template <typename DT>
inline
memory_default_callback<DT>::
memory_default_callback(memory_fast_access<DT>& fastAccess,
                        toplevel_memory_base& mem) :
  memory_callback_base(""),
  mFastAccess(fastAccess),
  mMemory(mem)
{
}

template <typename DT>
inline
void
memory_default_callback<DT>::
execute(tlm::tlm_generic_payload& trans, sc_core::sc_time& t)
{
  mFastAccess.transport(trans);
  if (trans.is_read()) {
    t += mMemory.get_default_read_latency();

  } else {
    assert(trans.is_write());
    t += mMemory.get_default_write_latency();
  }
}

template <typename DT>
inline
bool
memory_default_callback<DT>::
has_default_behavior() const
{
  return true;
}

template <typename DT>
inline
bool
memory_default_callback<DT>::
has_never_syncing_behavior() const
{
  return true;
}

}

#endif
