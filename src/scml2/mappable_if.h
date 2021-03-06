/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_MAPPABLE_IF_H
#define SCML2_MAPPABLE_IF_H

#include <string>

namespace tlm {
  class tlm_generic_payload;
  class tlm_dmi;
  class tlm_bw_direct_mem_if;
}

namespace sc_core {
  class sc_time;
}

namespace scml2 {

class mappable_if
{
public:
  virtual ~mappable_if();

  virtual std::string get_mapped_name() const = 0;

  virtual void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t) = 0;
  virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmiData) = 0;
  virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans) = 0;

  virtual void register_bw_direct_mem_if(tlm::tlm_bw_direct_mem_if* bwInterface) = 0;
  virtual void unregister_bw_direct_mem_if(tlm::tlm_bw_direct_mem_if* bwInterface) = 0;
};

inline
mappable_if::
~mappable_if()
{
}

}

#endif
