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

#include "scml_clock_enable.h"

scml_clock_enable::scml_clock_enable( const char * name ) :
  sc_signal< bool >( name )
{
}

scml_clock_enable::~scml_clock_enable()
{
}
