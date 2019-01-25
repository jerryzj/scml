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
 *
 */


scml_memory_base::TypeID scml_memory_operations< char >::getType() const
{ 
#ifdef __CHAR_UNSIGNED__
  return MEM_TYPE_UINT8; 
#else
  return MEM_TYPE_INT8; 
#endif
}


const char * scml_memory_operations< char >::getDocumentation() const
{ 
#ifdef __CHAR_UNSIGNED__
  return "Uint8 Memory";
#else
  return "Int8 Memory";
#endif
}

/************************************************************************
 * constructors for forward migration
 * behaviour is do as value_type does
 */

scml_memory_operations< char >::
scml_memory_operations() :
  scml_memory_operations_podt_common< char >()
{
}


scml_memory_operations< char >::
scml_memory_operations( value_type a ) :
  scml_memory_operations_podt_common< char >( a)
{
}

/************************************************************************
 * named default
 */

scml_memory_operations< char >::
scml_memory_operations( const char * n ) :
  scml_memory_operations_podt_common< char >( n)
{
}

/************************************************************************
 * constructors for memory array
 */

scml_memory_operations< char >::
scml_memory_operations( scml_memsize a ) :
  scml_memory_operations_podt_common< char >( a )
{
}


scml_memory_operations< char >::
scml_memory_operations( const char * n,
			scml_memsize a ) :
  scml_memory_operations_podt_common< char >( n, a )
{
}

/************************************************************************
 * alias constructors
 */

scml_memory_operations< char >::
scml_memory_operations( scml_memory_operations & a, 
			size_type offset, size_type size ) :
  scml_memory_operations_podt_common< char >( a, offset, size )
{
}


scml_memory_operations< char >::
scml_memory_operations( const char * n,
			scml_memory_operations & a, 
			size_type offset, size_type size ) :
  scml_memory_operations_podt_common< char >( n, a, offset, size )
{
}

/************************************************************************
 * virtual getter setter for instrumentation
 *
 * see scml_memory_base.h
 */


#ifdef __CHAR_UNSIGNED__

scml_memory_base::ErrorCode
scml_memory_operations< char >::
getRegisterValueUint8(uint8 & result, uint64 elementIndex)
{
  size_type i( elementIndex );
  if( !isValidIndex( i)) {
    return scml_memory_base::errorInvalidIndex;
  }
  result = this->readDebug(i);
  return scml_memory_base::noError;
}


scml_memory_base::ErrorCode
scml_memory_operations< char >::
setRegisterValueUint8(uint8 result, uint64 elementIndex)
{
  size_type i( elementIndex );
  if( !isValidIndex( i)) {
    return scml_memory_base::errorInvalidIndex;
  }
  this->writeDebug(result, i);
  return scml_memory_base::noError;
}


scml_memory_base::ErrorCode
scml_memory_operations< char >::
getRegisterValueVecUint8(::std::vector< uint8 > & result,
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

#else // __CHAR_UNSIGNED__


scml_memory_base::ErrorCode
scml_memory_operations< char >::
getRegisterValueInt8(int8 & result, uint64 elementIndex)
{
  size_type i( elementIndex );
  if( !isValidIndex( i)) {
    return scml_memory_base::errorInvalidIndex;
  }
  result = this->readDebug(i);
  return scml_memory_base::noError;
}


scml_memory_base::ErrorCode
scml_memory_operations< char >::
setRegisterValueInt8(int8 result, uint64 elementIndex)
{
  size_type i( elementIndex );
  if( !isValidIndex( i)) {
    return scml_memory_base::errorInvalidIndex;
  }
  this->writeDebug(result, i);
  return scml_memory_base::noError;
}


scml_memory_base::ErrorCode
scml_memory_operations< char >::
getRegisterValueVecInt8(::std::vector< int8 > & result,
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

#endif // __CHAR_UNSIGNED__
