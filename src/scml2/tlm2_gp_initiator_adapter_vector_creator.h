/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/
#ifndef SCML2_TLM2_GP_INITIATOR_ADAPTER_CREATOR_H
#define SCML2_TLM2_GP_INITIATOR_ADAPTER_CREATOR_H

#include <systemc.h>
#include <scml2/tlm2_gp_initiator_adapter.h>

namespace scml2 {

template <unsigned int BUSWIDTH, int N = 1, sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND>
class tlm2_gp_initiator_adapter_vector_creator
{
public:
  typedef tlm2_gp_initiator_adapter<BUSWIDTH, N, POL> adapter_type;
  typedef tlm::tlm_initiator_socket<BUSWIDTH, tlm::tlm_base_protocol_types, N, POL> socket_type;

  tlm2_gp_initiator_adapter_vector_creator(sc_core::sc_vector<socket_type>& sockets) : mSockets_core(&sockets), mSockets_scml2(0) {
  }

  tlm2_gp_initiator_adapter_vector_creator(scml2::vector<socket_type>& sockets) : mSockets_core(0), mSockets_scml2(&sockets) {
  }

  adapter_type* operator()(const char* prefix, size_t i) {
    if (mSockets_core != 0) {
      return new adapter_type(prefix, (*mSockets_core)[i]);
    } else {
      assert(mSockets_scml2 != 0);
      return new adapter_type(prefix, (*mSockets_scml2)[i]);
    }
  }

private:
  sc_core::sc_vector<socket_type>* mSockets_core;
  scml2::vector<socket_type>* mSockets_scml2;
};

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline tlm2_gp_initiator_adapter_vector_creator<BUSWIDTH, N, POL>
create_tlm2_gp_initiator_adapter_vector_creator(sc_core::sc_vector<tlm::tlm_initiator_socket<BUSWIDTH, tlm::tlm_base_protocol_types, N, POL> >& sockets) {
  return tlm2_gp_initiator_adapter_vector_creator<BUSWIDTH, N, POL>(sockets);
}

template <unsigned int BUSWIDTH, int N, sc_core::sc_port_policy POL>
inline tlm2_gp_initiator_adapter_vector_creator<BUSWIDTH, N, POL>
create_tlm2_gp_initiator_adapter_vector_creator(scml2::vector<tlm::tlm_initiator_socket<BUSWIDTH, tlm::tlm_base_protocol_types, N, POL> >& sockets) {
  return tlm2_gp_initiator_adapter_vector_creator<BUSWIDTH, N, POL>(sockets);
}


}

#endif
