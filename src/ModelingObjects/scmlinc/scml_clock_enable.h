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
 * Purpose of file : Definition of scml_clock_enable modeling object
 *
 */

#ifndef SCML_CLOCK_ENABLE_H
#define SCML_CLOCK_ENABLE_H

#include "systemc.h"
#include <scml2_logging/snps_vp_dll.h>

/************************************************************************
 * 
 */
class SNPS_VP_API scml_clock_enable : public sc_signal< bool > {
public:
  explicit scml_clock_enable( const char * name );
  ~scml_clock_enable();

private:
  // disable
  scml_clock_enable();
  scml_clock_enable( const scml_clock_enable & );
  scml_clock_enable & operator= ( const scml_clock_enable & );
};

#endif
