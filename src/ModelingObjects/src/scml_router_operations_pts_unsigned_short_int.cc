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

/************************************************************************
 * constructors for forward migration
 * behaviour is do as value_type does
 */

scml_router_operations< unsigned short int >::
scml_router_operations() :
  scml_router_operations_podt_common< unsigned short int >()
{
}


/************************************************************************
 * named default
 */

scml_router_operations< unsigned short int >::
scml_router_operations( const char * n ) :
  scml_router_operations_podt_common< unsigned short int >( n)
{
}

/************************************************************************
 * constructors for memory array
 */

scml_router_operations< unsigned short int >::
scml_router_operations( scml_memsize a ) :
  scml_router_operations_podt_common< unsigned short int >( a )
{
}


scml_router_operations< unsigned short int >::
scml_router_operations( const char * n,
                        scml_memsize a ) :
  scml_router_operations_podt_common< unsigned short int >( n, a )
{
}

/************************************************************************
 * virtual getter setter for instrumentation
 *
 * see scml_memory_base.h
 */

scml_memory_base::ErrorCode
scml_router_operations< unsigned short int >::
getRegisterValueUint16( uint16 & result,
                        uint64 elementIndex)
{
  size_type i( elementIndex );
  if( !isValidIndex( i)) {
    return scml_memory_base::errorInvalidIndex;
  }
  result = this->readDebug( i, 8, 0);
  result |= (this->readDebug( i, 8, 8) << 8);
  return scml_memory_base::noError;
}


scml_memory_base::ErrorCode
scml_router_operations< unsigned short int >::
setRegisterValueUint16( uint16 result,
                        uint64 elementIndex)
{
  size_type i( elementIndex );
  if( !isValidIndex( i)) {
    return scml_memory_base::errorInvalidIndex;
  }
  this->writeDebug( result, i, 8, 0);
  this->writeDebug( result >> 8, i, 8, 8);
  return scml_memory_base::noError;
}


scml_memory_base::ErrorCode
scml_router_operations< unsigned short int >::
getRegisterValueVecUint16( ::std::vector< uint16 > & result,
                           uint64 elementIndex,
                           uint32 elementCount) 
{
  size_type i( elementIndex );
  if( !isValidIndex( i)) {
    return scml_memory_base::errorInvalidIndex;
  }
  const size_type iEnd( i + elementCount );
  if( !isValidIndex( iEnd - 1)) {
    return scml_memory_base::errorInvalidIndex;
  }  
  result.clear();
  result.reserve( elementCount);
  for( ; i != iEnd; ++i ) {
    uint16 tmp = this->readDebug( i, 8, 0);
    tmp |= (this->readDebug( i, 8, 8) << 8);
    result.push_back(tmp);
  }
  return scml_memory_base::noError;
}
