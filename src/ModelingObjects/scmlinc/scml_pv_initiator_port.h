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
 * Purpose of file : 
 *
 */

#ifndef SCML_PV_INITIATOR_PORT_H
#define SCML_PV_INITIATOR_PORT_H

#include <PV/PV.h>
#include "scmlinc/scml_mapable_if.h"
#include "systemc.h"


#include <cassert>

template< typename DT = unsigned int, typename AT = unsigned int>
class scml_pv_initiator_port;

struct scml_memory_access_descriptor;

template <typename DT, typename AT>
class scml_pv_initiator_port : public PVInitiator_port< DT, AT>,
		               public scml_mapable_if
{
public:
  typedef AT address_type;
  typedef DT data_type;
  typedef PVReq<data_type, address_type> request_type;
  typedef PVResp<data_type> response_type;  

public:
  explicit scml_pv_initiator_port(const char * n,
			          scml_endian endian = scml_little_endian);
  virtual ~scml_pv_initiator_port();

  using scml_mapable_if::transport;
  virtual response_type transport(const request_type& req);

  scml_endian get_endianness() const;

public:
  virtual void start_of_simulation();


  virtual const char* getName() const { return this->name(); }

  //FIXME
  virtual void setPVInterfaceType(scml::PVInterfaceType pvInterfaceType);
  virtual scml::PVInterfaceType getPVInterfaceType() const;

private: 
  // disable
  scml_pv_initiator_port( const scml_pv_initiator_port & );
  scml_pv_initiator_port & operator= ( const scml_pv_initiator_port & );

private:

private:
  // data member
  scml_endian m_endian;
};

/************************************************************************
 * INLINE IMPLEMENTATIONS
 */

template <typename DT, typename AT>
inline
scml_pv_initiator_port<DT, AT>::
scml_pv_initiator_port(const char * n, scml_endian endian) :
  PVInitiator_port< DT, AT>(n),
  m_endian(endian)
{
}

template <typename DT, typename AT>
inline
scml_pv_initiator_port<DT, AT>::
~scml_pv_initiator_port()
{
}

template <typename DT, typename AT>
inline
void
scml_pv_initiator_port<DT, AT>::
start_of_simulation()
{
}

template <typename DT, typename AT>
inline
scml_endian 
scml_pv_initiator_port<DT, AT>::
get_endianness() const
{
  return m_endian;
}

template <typename DT, typename AT>
inline
typename scml_pv_initiator_port<DT, AT>::response_type
scml_pv_initiator_port<DT, AT>::
transport(const request_type& constReq)
{
    this->PVInitiator_port<DT, AT>::transport(constReq);
  return constReq.obtainResp();
}


template <typename DT, typename AT>
inline
void
scml_pv_initiator_port<DT, AT>::
setPVInterfaceType(scml::PVInterfaceType pvInterfaceType)
{
  if (pvInterfaceType == scml::PV_INTERFACE_UNKNOWN) {
    return;
  }
  assert(pvInterfaceType == getPVInterfaceType());
}

template <typename DT, typename AT>
inline
scml::PVInterfaceType
scml_pv_initiator_port<DT, AT>::
getPVInterfaceType() const
{
  typedef typename scml::Types2If<DT, AT>::result IF;
  return (scml::PVInterfaceType)scml::If2PVInterfaceType<IF>::result;
}

#endif
