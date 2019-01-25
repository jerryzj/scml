/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_WORD_ACCESS_UNROLLER_BASE_H
#define SCML2_WORD_ACCESS_UNROLLER_BASE_H

#include <tlm.h>
#include <scml2/tlm2_gp_extensions.h>
#include <scml2/utils.h>
#include <scml2_logging/snps_vp_dll.h>

namespace sc_core {
  class sc_time;
}

namespace scml2 {

template <typename DT>
class SNPS_VP_API word_access_unroller_base
{
public:
  virtual ~word_access_unroller_base();

  virtual bool process_unrolled_access(DT& data,
                                       const DT& byteEnables,
                                       sc_core::sc_time& t,
                                       const tlm2_gp_extensions&) = 0;
  void unroll_to_word_access(tlm::tlm_generic_payload& trans, sc_core::sc_time& t);

private:
  bool do_access(unsigned char* data,
                 unsigned int size,
                 unsigned int offset,
                 bool readNotWrite,
                 const DT& byteEnables,
                 sc_core::sc_time& t,
                 const tlm2_gp_extensions&);
};

}

#endif
