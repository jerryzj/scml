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

#ifndef SCML_POST_IF_H
#define SCML_POST_IF_H

#include "systemc.h"

template< typename DT, typename AT > class scml_transaction_request;

template< typename DT = unsigned int, typename AT = unsigned int>
class scml_post_if;

template< typename DT, typename AT >
class scml_post_if : virtual public sc_interface {
public:
  typedef DT data_type;
  typedef AT address_type;

public:
  virtual bool post( scml_transaction_request< DT, AT> & ) = 0;

protected:
  scml_post_if() {}
  virtual ~scml_post_if() {} 

private: 
  // disable
  scml_post_if( const scml_post_if & );
  scml_post_if & operator= ( const scml_post_if & );
};

#endif
