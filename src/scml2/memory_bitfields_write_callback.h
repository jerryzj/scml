/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MEMORY_BITFIELDS_WRITE_CALLBACK_H
#define SCML2_MEMORY_BITFIELDS_WRITE_CALLBACK_H

#include <scml2/memory_callback_base.h>
#include <scml2/word_access_unroller_base.h>
#include <scml2/utils.h>
#include <scml2_logging/snps_vp_dll.h>

namespace sc_core {
  class sc_time;
}

namespace tlm {
  class tlm_generic_payload;
}

namespace scml2 {

class tlm2_gp_extensions;
template <typename DT> class reg;
template <typename DT> class bitfield;

template <typename DT>
class memory_bitfields_write_callback :
  public memory_callback_base,
  public word_access_unroller_base<DT>
{
public:
  memory_bitfields_write_callback(reg<DT>& r);

  virtual void execute(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);
  virtual bool process_unrolled_access(DT& data,
                                       const DT& byteEnables,
                                       sc_core::sc_time& t,
                                       const tlm2_gp_extensions&);
  virtual bool has_never_syncing_behavior() const;

private:
  reg<DT>& mRegister;
};

template <typename DT>
inline
memory_bitfields_write_callback<DT>::
memory_bitfields_write_callback(reg<DT>& r) :
  memory_callback_base(""),
  word_access_unroller_base<DT>(),
  mRegister(r)
{
}

#define SCML2_INSTANTIATE_TEMPLATE(type) \
   SNPS_VP_API_TMPL_CLASS(memory_bitfields_write_callback<type >;)
SCML2_FOR_EACH_DT(SCML2_INSTANTIATE_TEMPLATE)
#undef SCML2_INSTANTIATE_TEMPLATE

}

#endif
