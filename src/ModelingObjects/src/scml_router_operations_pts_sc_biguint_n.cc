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
 * Purpose of file : Definition of scml_memory modeling object
 *
 */

#include "scmlinc/scml_router_operations.h"
#include <sstream>

/************************************************************************
 * constructors for forward migration
 * behaviour is do as value_type does
 */

template <int N>
scml_router_operations< sc_dt::sc_biguint<N> >::
scml_router_operations() :
  scml_router_operations_podt_common< sc_dt::sc_biguint<N> >()
{
}


/************************************************************************
 * named default
 */

template <int N>
scml_router_operations< sc_dt::sc_biguint<N> >::
scml_router_operations( const char * n ) :
  scml_router_operations_podt_common< sc_dt::sc_biguint<N> >( n)
{
}

/************************************************************************
 * constructors for memory array
 */

template <int N>
scml_router_operations< sc_dt::sc_biguint<N> >::
scml_router_operations( scml_memsize a ) :
  scml_router_operations_podt_common< sc_dt::sc_biguint<N> >( a )
{
}


template <int N>
scml_router_operations< sc_dt::sc_biguint<N> >::
scml_router_operations( const char * n,
                        scml_memsize a ) :
  scml_router_operations_podt_common< sc_dt::sc_biguint<N> >( n, a )
{
}

/************************************************************************
 * documentation
 */

template <int N>
std::string
scml_router_operations< sc_dt::sc_biguint<N> >::
getDocumentationImp() const
{
  std::ostringstream s;
  s << "UInt" << N << " Router";
  return s.str();
}

template <int N>
const char *
scml_router_operations< sc_dt::sc_biguint<N> >::
getDocumentation() const
{
  static const std::string s = getDocumentationImp();
  return s.c_str();
}

/************************************************************************
 * virtual getter setter for instrumentation
 *
 * see scml_memory_base.h
 */


/************************************************************************
 * Explicit Instantiations
 */

template class SNPS_VP_API scml_router_operations< sc_dt::sc_biguint<128> >;
template class SNPS_VP_API scml_router_operations< sc_dt::sc_biguint<256> >;
template class SNPS_VP_API scml_router_operations< sc_dt::sc_biguint<512> >;
