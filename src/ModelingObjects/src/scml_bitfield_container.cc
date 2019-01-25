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
 * Purpose of file : Implementation of bitfield containers
 *
 */

#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable: 4804 ) //  unsafe use of type 'bool' in operation
#pragma warning( disable: 4305 ) //  'initializing' : truncation from 'int' to 'bool'
#endif
#include "scml_bitfield_container.h"
#ifdef _WIN32
#pragma warning( pop )
#endif
#include "scml_memory_user.h"
#include "scml_types.h"

using namespace scml;

scml_bitfield_container::~scml_bitfield_container()
{
}

scml_bitfield_container::scml_bitfield_container() : 
  fields() 
{
}

bool
scml_bitfield_container::registerBitfields( scml_bitfield * b )
{
  fields.push_back( b);
  return true;
}

bool 
scml_bitfield_container::getBitfieldPointers( bitfield_pointer_container & v ) const
{ 
  v.clear();
  v.reserve( fields.size());
  for( const_iterator_type it = fields.begin(), end = fields.end();
       it != end; ++it) {
    v.push_back( *(it));
  }
  return true;
}

scml_bitfield_container::scml_bitfield_container_entry
scml_bitfield_container::findBitfield(const std::string& name) const
{
  for( const_iterator_type it = fields.begin(), end = fields.end();
       it != end; ++it) {
    if ((*it)->getName() == name) {
      return *it;
    }
  }

  return 0;
}

scml_bitfield_container::scml_bitfield_container_entry
scml_bitfield_container::findOverlap(scml_bitfield_container_entry b) const
{
  for( const_iterator_type it = fields.begin(), end = fields.end();
       it != end; ++it) {
    if ((*it)->getOffset() < b->getOffset() + b->getSize() &&
        b->getOffset() < (*it)->getOffset() + (*it)->getSize()) {
      return *it;
    }
  }

  return 0;
}

/************************************************************************
 * TODO FIX ME refactor these away
 */

bool
scml_bitfield_container::updateReadBlockingProperty( bool )
{
  // TODO FIX ME refactor this away
  assert(0);
  return true;
}

bool
scml_bitfield_container::updateWriteBlockingProperty( bool )
{
  // TODO FIX ME refactor this away
  assert(0);
  return true;
}

scml_bitfield_container::value_type
scml_bitfield_container::getBitfieldValue( size_type, size_type)
{
  // TODO FIX ME refactor this away
  assert(0);
  return 0;
}

void 
scml_bitfield_container::setBitfieldValue( value_type, size_type, size_type)
{
  // TODO FIX ME refactor this away
  assert(0);
}

/************************************************************************
 * 
 */

bool
scml_bitfield_container::hasReadCallbacks() {
  iterator_type it( fields.begin());
  iterator_type end( fields.end());
  for( ; it != end ; ++it ) {
    scml_bitfield_container_entry b( *it);
    if( b->hasRegisteredReadCallBack()) {
      return true;
    }
  }
  return false;
}

bool
scml_bitfield_container::hasWriteCallbacks() {
  iterator_type it( fields.begin());
  iterator_type end( fields.end());
  for( ; it != end ; ++it ) {
    scml_bitfield_container_entry b( *it);
    if( b->hasRegisteredWriteCallBack()) {
      return true;
    }
  }
  return false;
}

bool
scml_bitfield_container::hasCallbacks() {
  iterator_type it( fields.begin());
  iterator_type end( fields.end());
  for( ; it != end ; ++it ) {
    scml_bitfield_container_entry b( *it);
    if( b->hasRegisteredReadCallBack() || b->hasRegisteredWriteCallBack()) {
      return true;
    }
  }
  return false;
}

bool
scml_bitfield_container::hasReadBehaviourAttached()
{
  iterator_type it( fields.begin());
  iterator_type end( fields.end());
  for( ; it != end ; ++it ) {
    scml_bitfield_container_entry b( *it);
    if( b->hasReadBehaviourAttached()) {
      return true;
    }
  }
  return false;
}

bool
scml_bitfield_container::hasWriteBehaviourAttached()
{
  iterator_type it( fields.begin());
  iterator_type end( fields.end());
  for( ; it != end ; ++it ) {
    scml_bitfield_container_entry b( *it);
    if( b->hasWriteBehaviourAttached()) {
      return true;
    }
  }
  return false;
}

bool
scml_bitfield_container::hasBehaviourAttached()
{
  iterator_type it( fields.begin());
  iterator_type end( fields.end());
  for( ; it != end ; ++it ) {
    scml_bitfield_container_entry b( *it);
    if( b->hasReadBehaviourAttached() || b->hasWriteBehaviourAttached()) {
      return true;
    }
  }
  return false;
}

/************************************************************************
 * read and write
 */

#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable: 4800 ) //  unsafe use of type 'bool' in operation
#pragma warning( disable: 4804 ) //  unsafe use of type 'bool' in operation
#pragma warning( disable: 4805 ) //  unsafe mix of type 'bool' and type 'int' in operation
#endif
template <typename DT>
DT scml_bitfield_container::read( DT& a,
				  unsigned int accessSize,
				  unsigned int offset) {
  scml_memory_user & inst( scml_memory_user::instance());
  const unsigned int firstAffectedBit( offset);
  const unsigned int lastAffectedBit( offset + accessSize - 1);
  DT resultMask( 0);
  iterator_type it( fields.begin());
  iterator_type end( fields.end());
  const DT readMask(scml_memory_utils::getBitsMask<DT>(accessSize) << offset);
  for( ; it != end ; ++it ) {
    scml_bitfield_container_entry b( *it);
    const unsigned int firstBit( b->getOffset());
    const unsigned int sizeBitF( b->getSize());
    const unsigned int lastBit( firstBit + sizeBitF - 1);
    if(( lastBit < firstAffectedBit) || (firstBit > lastAffectedBit)) {
      // do nothing
    } else {
      inst.set_last_bitfield_pointer( b);      
      value_type r;
      if (!inst.is_debug_access() && !b->is_reading_allowed()) {
        // reading of a write-only register should return 0
        r = 0;

      } else if( b->hasRegisteredReadCallBack()) {
	inst.set_last_user_id( b->getReadUserID());
          r = (((b->readModule)->*(b->rcb) ) ()) &
            scml_memory_utils::getBitsMask<value_type>(b->getSize());
      } else {
        continue;
      }
      if (!inst.is_debug_access()) {
        b->put( r);
      }
      const DT mask = scml_memory_utils::getBitsMask<DT>(b->getSize()) << b->getOffset();
      a &= ~(mask & readMask);
      a |= (convert<value_type, DT>(r) <<  b->getOffset()) & (mask & readMask);
      resultMask |= mask;	
    }
  }
  return resultMask;
}

template <typename DT>
DT scml_bitfield_container::write( const DT& a,
				   unsigned int accessSize,
				   unsigned int offset) {
  scml_memory_user & inst( scml_memory_user::instance());
  const unsigned int firstAffectedBit( offset);
  const unsigned int lastAffectedBit( offset + accessSize - 1);
  DT resultMask( 0);
  iterator_type it( fields.begin());
  iterator_type end( fields.end());
  for( ; it != end ; ++it ) {
    scml_bitfield_container_entry b( *it);
    const unsigned int firstBit( b->getOffset());
    const unsigned int sizeBitF( b->getSize());
    const unsigned int lastBit( firstBit + sizeBitF - 1);
    if(( lastBit < firstAffectedBit) || (firstBit > lastAffectedBit)) {
      // do nothing
    } else {
      inst.set_last_bitfield_pointer( b);
      if (!inst.is_debug_access() && !b->is_writing_allowed()) {
        // writing a read-only register should do nothing

      } else {
        const value_type oldVal = value_type( b->get());
        const DT mask = scml_memory_utils::getBitsMask<DT>(accessSize);
        const DT updVal( a & mask);
        DT v = convert<value_type, DT>(oldVal) << firstBit;
        v &= ~(mask << firstAffectedBit);
        v |= updVal << firstAffectedBit;
        v >>= firstBit;
        v &= scml_memory_utils::getBitsMask<DT>(sizeBitF);
        
        if( b->hasRegisteredWriteCallBack()) {
          inst.set_last_user_id( b->getWriteUserID());
            ((b->writeModule)->*(b->wcb) ) ( convert<DT, value_type>(v));
        } else {
          b->put( convert<DT, value_type>(v));
        }
      }
      resultMask |= scml_memory_utils::getBitsMask<DT>(b->getSize()) << b->getOffset();	
    }
  }
  return resultMask;
}

#ifdef _WIN32
#pragma warning( pop )
#endif
/************************************************************************
 * Explicit Instantiations
 */

#define SCML_CONTAINER_READ_WRITE(Type) \
  template \
  Type scml_bitfield_container::read<Type >( Type& wData, unsigned int accessSize, unsigned int offset); \
  template \
  Type scml_bitfield_container::write<Type >( const Type& wData, unsigned int accessSize, unsigned int offset);

SCML_CONTAINER_READ_WRITE(bool);
SCML_CONTAINER_READ_WRITE(char);
// SCML_CONTAINER_READ_WRITE(float);
// SCML_CONTAINER_READ_WRITE(double);
// SCML_CONTAINER_READ_WRITE(long double);

SCML_CONTAINER_READ_WRITE(signed char);
SCML_CONTAINER_READ_WRITE(signed short int);
SCML_CONTAINER_READ_WRITE(signed int);
SCML_CONTAINER_READ_WRITE(signed long int);
SCML_CONTAINER_READ_WRITE(signed long long);

SCML_CONTAINER_READ_WRITE(unsigned char);
SCML_CONTAINER_READ_WRITE(unsigned short int);
SCML_CONTAINER_READ_WRITE(unsigned int);
SCML_CONTAINER_READ_WRITE(unsigned long int);
SCML_CONTAINER_READ_WRITE(unsigned long long);

SCML_CONTAINER_READ_WRITE(sc_dt::sc_biguint<128>);
SCML_CONTAINER_READ_WRITE(sc_dt::sc_biguint<256>);
SCML_CONTAINER_READ_WRITE(sc_dt::sc_biguint<512>);

#undef SCML_CONTAINER_READ_WRITE
