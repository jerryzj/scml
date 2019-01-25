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
 * Purpose of file : Definition of bitfield container
 *
 */

#ifndef SCML_BITFIELD_CONTAINER_H
#define SCML_BITFIELD_CONTAINER_H

#include <vector>
#include "scmlinc/scml_bitfield.h"
#include "scmlinc/scml_memory_utils.h"
#include <scml2_logging/snps_vp_dll.h>

class SNPS_VP_API scml_bitfield_container : public scml_bitfield_container_if {
public: 
  typedef scml_bitfield_container_if::value_type value_type;
  typedef scml_bitfield_container_if::size_type size_type;
  typedef scml_bitfield_container_if::scml_bitfield_container_entry scml_bitfield_container_entry;
  // constructor & destructor
  scml_bitfield_container();
  virtual ~scml_bitfield_container();

  // see scml_bitfield_container_if
  virtual bool registerBitfields( scml_bitfield_container_entry );
  virtual bool updateReadBlockingProperty( bool isBlocking );
  virtual bool updateWriteBlockingProperty( bool isBlocking );
  virtual value_type getBitfieldValue( size_type offset, size_type size);
  virtual void setBitfieldValue( value_type, size_type offset, size_type size);

  // bit fields support read & write
  template <typename DT>
  DT read( DT& rData, unsigned int accessSize, unsigned int offset);
  template <typename DT>
  DT write( const DT& wData, unsigned int accessSize, unsigned int offset);
 
  typedef scml_bitfield_base::bitfield_pointer_container bitfield_pointer_container;
  bool getBitfieldPointers( bitfield_pointer_container & ) const;
  bool hasCallbacks();
  bool hasReadCallbacks();
  bool hasWriteCallbacks();

  bool hasReadBehaviourAttached();
  bool hasWriteBehaviourAttached();
  bool hasBehaviourAttached();

  scml_bitfield_container_entry findBitfield(const std::string& name) const;
  scml_bitfield_container_entry findOverlap(scml_bitfield_container_entry b) const;

private:
  // disable
  scml_bitfield_container( const scml_bitfield_container & );
  scml_bitfield_container & operator= ( const scml_bitfield_container & );

private:
  // data members
  typedef ::std::vector< scml_bitfield_container_entry > container_type;
  typedef container_type::iterator iterator_type;
  typedef container_type::const_iterator const_iterator_type;

  container_type fields;
};

#endif
