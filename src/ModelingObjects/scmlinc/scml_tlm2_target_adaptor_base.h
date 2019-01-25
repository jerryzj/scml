/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
/*
 * Modifications :
 *
 * Purpose of file : Definition of the scml_tlm2_target_adaptor_base
 *
 */

#ifndef SCML_TLM2_TARGET_ADAPTOR_BASE_H
#define SCML_TLM2_TARGET_ADAPTOR_BASE_H

#include "scmlinc/scml_mapable_if.h"
#include <scml2_logging/snps_vp_dll.h>
#include <tlm.h>

class SNPS_VP_API scml_tlm2_target_adaptor_base : public sc_core::sc_module
{
public:
  sc_core::sc_port<scml_mapable_if> pOut;

public:
  SC_HAS_PROCESS(scml_tlm2_target_adaptor_base);
  scml_tlm2_target_adaptor_base(sc_core::sc_module_name name, unsigned int busWidth);
  ~scml_tlm2_target_adaptor_base();

  void set_endianness(scml_endian e);
  scml_endian get_endianness() const;

protected:
  void forwardTransaction(tlm::tlm_generic_payload& trans, bool isDebug);
  template <typename IF>
  void forwardTransactionImp(tlm::tlm_generic_payload& trans, bool isDebug);
  template <typename IF>
  void unrollTransaction(tlm::tlm_generic_payload& trans, bool isDebug);

  bool forwardDMIRequest(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data);

private:
  const unsigned int mBusWidth;
  bool mHasHostEndianness;
};

#endif
