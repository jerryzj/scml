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

#ifndef SCML_CLOCK_COUNTER_H
#define SCML_CLOCK_COUNTER_H

#include <scml2_logging/stream.h>
#include <scml2_logging/snps_vp_dll.h>
#include <string>
#include <scml_clock/scml_clock.h>

/************************************************************************
 * 
 */
class SNPS_VP_API scml_clock_counter {
public:
  typedef sc_dt::uint64 data_type;
private:
  typedef data_type T;

public:
  // constructor & destructor
  explicit scml_clock_counter( const char * name );
  scml_clock_counter( const char * name, scml_clock_if & ); 
  virtual ~scml_clock_counter();

  // binding
  void bind( scml_clock_if & );
  void operator()( scml_clock_if & );

  // counter manipulation
  data_type get_count() const;
  void set_count( data_type );
 
  // read/write interface
  T read() const;
  void write( const T a );
  
  operator T() const;
  T operator = ( const T );

  // sc_object like functions
  virtual const char * kind() const;
  const char * name() const;

private:
  // disable
  scml_clock_counter();
  scml_clock_counter( const scml_clock_counter & );
  scml_clock_counter & operator= ( const scml_clock_counter & );

private:
  // data members
  mutable scml2::logging::stream mWarning;
  scml_clock_if * mCif;
  data_type mLastWrittenCountValue;
  ::std::string m_name;
};

#endif
