/***************************************************************************
 * Copyright 1996-2017 Synopsys, Inc.
 *
 * This Synopsys software and all associated documentation are proprietary
 * to Synopsys, Inc. and may only be used pursuant to the terms and
 * conditions of a written license agreement with Synopsys, Inc.
 * All other use, reproduction, modification, or distribution of the
 * Synopsys software or the associated documentation is strictly prohibited.
 ***************************************************************************/

#include "testInitiator.h"
#include "HWaccelerator_Target_PV.h"

int sc_main( int, char **) {
  HWaccelerator_Target_PV I_HWaccelerator_Target_PV( "I_HWaccelerator_Target_PV");
  testInitiator aTestInitiator( "aTestInitiator");
  aTestInitiator.p( I_HWaccelerator_Target_PV.p_PV );

  const sc_time runTime( 10, SC_NS );
  sc_start( runTime );
  return 0;
}
