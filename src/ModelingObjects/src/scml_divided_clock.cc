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
 * Purpose of file : Definition of scml_clock_counter modeling object
 *
 */

#include "scml_divided_clock.h"

/************************************************************************
 * constructor & destructor
 */

scml_divided_clock::scml_divided_clock( const char * name,
					unsigned int period_multiplier,
					unsigned int start_multiplier ) :
  scml_clock( name, (scml_clock*)0, (double)period_multiplier, (double)start_multiplier ),
  m_constuctor_period_multiplier( period_multiplier ),
  m_current_divider( 1 )
{
}

scml_divided_clock::scml_divided_clock( const char * name,
					scml_clock_if & clk,
					unsigned int period_multiplier,
					unsigned int start_multiplier ) :
  scml_clock( name, clk, period_multiplier, start_multiplier ),
  m_constuctor_period_multiplier( period_multiplier ),
  m_current_divider( 1 )
{
}

scml_divided_clock::scml_divided_clock( const char * name,
					sc_in< bool > & clk,
					unsigned int period_multiplier,
					unsigned int start_multiplier,
					bool allow_stubbed ) :
  scml_clock( name, clk, period_multiplier, start_multiplier, allow_stubbed ),
  m_constuctor_period_multiplier( period_multiplier ),
  m_current_divider( 1 )
{
}

scml_divided_clock::~scml_divided_clock()
{
}

/************************************************************************
 * binding
 */

void scml_divided_clock::bind( scml_clock & clk)
{
  set_master( clk);
}

void scml_divided_clock::bind( sc_in< bool> & s)
{
  set_master( s);
}

void scml_divided_clock::operator()( scml_clock & c)
{
  bind( c);
}

void scml_divided_clock::operator()( sc_in< bool> & s)
{
  bind( s);
}

/************************************************************************
 * constructor & destructor
 */
void scml_divided_clock::set_divider( unsigned int a )
{
  m_current_divider = a;
  set_period_multiplier(a*m_constuctor_period_multiplier);
} 

unsigned int scml_divided_clock::get_divider() const
{
  return m_current_divider;
}

/************************************************************************
 * see scml_clock
 * restricted access
 */
void scml_divided_clock::set_period( const sc_time & t)
{
  scml_clock::set_period( t);
}

