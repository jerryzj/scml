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

#ifndef SCML_DIVIDED_CLOCK_H
#define SCML_DIVIDED_CLOCK_H

#include <scml_clock/scml_clock.h>
#include <scml2_logging/snps_vp_dll.h>

/************************************************************************
 * 
 */
class SNPS_VP_API scml_divided_clock : public scml_clock {
public:
  explicit scml_divided_clock( const char * name,
			       unsigned int period_multiplier = 1,
			       unsigned int start_multiplier = 0 );  

  scml_divided_clock( const char * name,
		      scml_clock_if & clk_if,
		      unsigned int period_multiplier = 1,
		      unsigned int start_multiplier = 0 );

  scml_divided_clock( const char * name,
		      sc_in< bool > & clk,
		      unsigned int period_multiplier = 1,
		      unsigned int start_multiplier = 0,
		      bool allow_stubbed = false);  

  virtual ~scml_divided_clock();

  // binding
  void bind( scml_clock & );
  void bind( sc_in< bool > & );
  void operator()( scml_clock & );
  void operator()( sc_in< bool > & );

  // clock period can be changed in multiples of parent clock
  void set_divider( unsigned int );
  unsigned int get_divider() const;
protected:
  virtual void set_period( const sc_time & t );
  
private:
  // data members
  const unsigned int m_constuctor_period_multiplier;
  unsigned int m_current_divider;

private:
  // disable
  scml_divided_clock();
  scml_divided_clock( const scml_divided_clock & );
  scml_divided_clock & operator= ( const scml_divided_clock & );
};

#endif
