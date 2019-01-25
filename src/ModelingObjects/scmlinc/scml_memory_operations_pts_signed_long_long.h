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

#ifndef SCML_MEMORY_OPERATIONS_PTS_SIGNED_LONG_LONG_H
#define SCML_MEMORY_OPERATIONS_PTS_SIGNED_LONG_LONG_H

/************************************************************************
 * partial template specialisation
 *
 * see scml_memory.h 
 *
 */

template< > 
class SNPS_VP_API scml_memory_operations< signed long long > : 
  public scml_memory_operations_podt_common< signed long long >
{
public:
  typedef signed long long value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type* iterator;
  typedef const value_type* const_iterator;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef unsigned long long size_type;
  typedef ptrdiff_t difference_type;

  typedef scml_memory_operations< value_type > this_type;
  typedef this_type & this_reference_type;

  typedef scml_memory_base::uint64 uint64;

  typedef scml_memory_base::ErrorCode ErrorCode;

  // constructors & destructor
  scml_memory_operations();
  scml_memory_operations( value_type ); // acceptable conversion
  explicit scml_memory_operations( const char * n );
  explicit scml_memory_operations( scml_memsize );  
  scml_memory_operations( const char * n, scml_memsize );
  virtual ~scml_memory_operations() {}

  // alias constructors
  scml_memory_operations( scml_memory_operations & a,
			  size_type offset, size_type size );
  scml_memory_operations( const char * n,
			  scml_memory_operations & a, 
			  size_type offset, size_type size );

  // value_type variable behaviour
  inline this_reference_type operator=( value_type );
private:
  // disable copy and assignment
  this_reference_type operator=( const scml_memory_operations & );
  scml_memory_operations( const scml_memory_operations & );

public:
  // virtuals queries for clients for scml_memory_base
  virtual TypeID getType() const { return MEM_TYPE_INT64; }
  virtual bool isWriteable() const { return true; }
  virtual const char * getDocumentation() const { return "Int64 Memory";}
  
  // virtual getter setter for instrumentation
  virtual ErrorCode getRegisterValueInt64( int64 & result,
						       uint64 elementIndex = 0);
  virtual ErrorCode setRegisterValueInt64( int64 result,
					   uint64 elementIndex = 0);
  virtual ErrorCode getRegisterValueVecInt64( ::std::vector< int64 > & result,
					      uint64 elementIndex = 0,
					      uint32 elementCount = 1);
  
#if __WORDSIZE == 64
  // The following methods should be avoided since they are not portable
  // between 32 bit and 64 bit hosts.
  virtual ErrorCode getRegisterValueInt64(signed long int & result, uint64 elementIndex = 0);
  virtual ErrorCode getRegisterValueVecInt64(::std::vector< signed long int > & result,
					     uint64 elementIndex = 0,
					     uint32 elementCount = 1);
#endif  
};

inline scml_memory_operations< signed long long >::this_reference_type
scml_memory_operations< signed long long >::operator= ( value_type a )
{
  put( a);
  return *this;
}

#endif
