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

#include "scmlinc/scml_memory_operations.h"

/************************************************************************
 * constructors for forward migration
 * behaviour is do as value_type does
 */

scml_memory_operations< signed int >::
scml_memory_operations() :
  scml_memory_operations_podt_common< signed int >()
{
}


scml_memory_operations< signed int >::
scml_memory_operations( value_type a ) :
  scml_memory_operations_podt_common< signed int >( a)
{
}

/************************************************************************
 * named default
 */

scml_memory_operations< signed int >::
scml_memory_operations( const char * n ) :
  scml_memory_operations_podt_common< signed int >( n)
{
}

/************************************************************************
 * constructors for memory array
 */

scml_memory_operations< signed int >::
scml_memory_operations( scml_memsize a ) :
  scml_memory_operations_podt_common< signed int >( a )
{
}


scml_memory_operations< signed int >::
scml_memory_operations( const char * n,
			scml_memsize a ) :
  scml_memory_operations_podt_common< signed int >( n, a )
{
}

/************************************************************************
 * alias constructors
 */

scml_memory_operations< signed int >::
scml_memory_operations( scml_memory_operations & a, 
			size_type offset, size_type size ) :
  scml_memory_operations_podt_common< signed int >( a, offset, size )
{
}


scml_memory_operations< signed int >::
scml_memory_operations( const char * n,
			scml_memory_operations & a, 
			size_type offset, size_type size ) :
  scml_memory_operations_podt_common< signed int >( n, a, offset, size )
{
}

/************************************************************************
 * virtual getter setter for instrumentation
 *
 * see scml_memory_base.h
 */

scml_memory_base::ErrorCode
scml_memory_operations< signed int >::
getRegisterValueInt32(int32 & result, uint64 elementIndex)
{
  size_type i( elementIndex );
  if( !isValidIndex( i)) {
    return scml_memory_base::errorInvalidIndex;
  }
  result = this->readDebug(i);
  return scml_memory_base::noError;
}


scml_memory_base::ErrorCode
scml_memory_operations< signed int >::
setRegisterValueInt32(int32 result, uint64 elementIndex)
{
  size_type i( elementIndex );
  if( !isValidIndex( i)) {
    return scml_memory_base::errorInvalidIndex;
  }
  this->writeDebug(result, i);
  return scml_memory_base::noError;
}


scml_memory_base::ErrorCode
scml_memory_operations< signed int >::
getRegisterValueVecInt32(::std::vector< int32 > & result,
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
    result.push_back(this->readDebug(i));
  }
  return scml_memory_base::noError;
}


#if __WORDSIZE != 64

scml_memory_base::ErrorCode
scml_memory_operations< signed int >::
getRegisterValueInt32(signed long & result, uint64 elementIndex)
{
  size_type i( elementIndex );
  if( !isValidIndex( i)) {
    return scml_memory_base::errorInvalidIndex;
  }
  result = this->readDebug(i);
  return scml_memory_base::noError;
}


scml_memory_base::ErrorCode
scml_memory_operations< signed int >::
getRegisterValueVecInt32(::std::vector< signed long > & result,
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
    result.push_back(this->readDebug(i));
  }
  return scml_memory_base::noError;
}

#endif  
