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
 * Purpose of file : Definition of scml_memsize modeling object
 *
 */

#ifndef SCML_MEMSIZE_H
#define SCML_MEMSIZE_H

#include <stddef.h>

class scml_memsize {
public:
  typedef unsigned long long size_type;

  explicit scml_memsize(size_type a) : s( a) {}

  // default OK
  // ~scml_memsize();  
  // scml_memsize( const scml_memsize & );
  // scml_memsize & operator= ( const scml_memsize & );

  operator size_type() const { return s; }

private:
  // disable
  scml_memsize();

private:
  // data member
  size_type s;
};

#endif
