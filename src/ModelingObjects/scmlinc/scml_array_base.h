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
 * Purpose of file : 
 *
 */

#ifndef SCML_ARRAY_BASE_H
#define SCML_ARRAY_BASE_H

#include <cstddef>
#include <vector>
#include <set>
#include <systemc.h>
#include <scml2_logging/snps_vp_dll.h>

#include "scmlinc/scml_array_notify_if.h"

class SNPS_VP_API scml_array_base : public sc_object {
public:
  typedef size_t size_type;
  typedef scml_array_base this_type;
  typedef this_type * this_pointer_type ;
  typedef this_type & this_reference_type ;

public:
  // non-blocking synchronization
  bool nb_claim_data( size_type index, size_type length = 1);
  bool nb_can_claim_data( size_type index, size_type length = 1) const;
  const sc_event & ok_to_claim_data() const;
  
  bool nb_claim_space( size_type index, size_type length = 1);
  bool nb_can_claim_space( size_type index, size_type length = 1) const;
  const sc_event & ok_to_claim_space() const;
  
  bool nb_release_data( size_type index, size_type length = 1);
  bool nb_release_space( size_type index, size_type length = 1);

  // blocking synchronization: 
  void claim_data( size_type index, size_type length = 1);
  void claim_space( size_type index, size_type length = 1);

public:
  // subscribe to claim notification
  typedef scml_array_notify_if * subscribers_type;
  void subscribe_claim_data( subscribers_type );
  void subscribe_claim_space( subscribers_type );

public:
  // queries
  size_type get_size() const;

  // see sc_object
  virtual void print(::std::ostream & ) const;
  virtual void dump(::std::ostream & ) const;
  virtual const char * kind() const;

public:
  typedef void * generic_data_pointer_type;
  virtual generic_data_pointer_type get_storage_pointer() const;

  virtual bool get( generic_data_pointer_type destination, size_type index,  
		    unsigned int acSiz, unsigned int offSet) const;
  virtual bool get( generic_data_pointer_type destination, size_type index) const;
  virtual bool put( generic_data_pointer_type source, size_type index);
  virtual bool put( generic_data_pointer_type source, size_type index,
		    unsigned int acSiz, unsigned int offSet);

  void repackage( size_type destIndex,
		  unsigned int destDataSize,			    
		  this_reference_type srcArray,
		  size_type srcIndex,
		  unsigned int srcDataSize,
		  unsigned int srcDataElements );

  // info on last release
  size_type get_last_released_data_index() const;
  size_type get_last_released_data_length() const;
  size_type get_last_released_space_index() const;
  size_type get_last_released_space_length() const;

protected:
  explicit scml_array_base( size_type n );
  scml_array_base(const char* name, size_type n);
  virtual ~scml_array_base();  
  
protected:
  bool setDataValid( size_type index, size_type length = 1);

  bool isValidData( size_type index ) const;
  bool isValidDataAll( size_type index, size_type length) const;
  bool isValidDataAtLeastOne( size_type index, size_type length) const;

  bool setDataInvalid( size_type index, size_type length = 1);

  bool lockSpace( size_type index, size_type length = 1);

  bool isLockedSpace( size_type index ) const;
  bool isLockedSpaceAll( size_type index, size_type length) const;
  bool isLockedSpaceAtLeastOne( size_type index, size_type length) const;

  bool unlockSpace( size_type index, size_type length = 1);

  // notify subscribers
  void notify_claim_data_subscribers();
  void notify_claim_space_subscribers();

private:
  // disable
  scml_array_base( const scml_array_base & );
  scml_array_base & operator= ( const scml_array_base & );

private:
  // data member
  ::std::vector< bool> mLockedSpace;
  ::std::vector< bool> mValidData;
  sc_event m_ok_to_claim_data;
  sc_event m_ok_to_claim_space;

  typedef ::std::set< subscribers_type > subscribers_container_type;
  typedef subscribers_container_type::iterator subscribers_iterator_type;

  subscribers_container_type mClaimDataSubscribers;
  subscribers_container_type mClaimSpaceSubscribers;

  // TODO FIXME
  // this only works for one release per delta cycle at least for the event
  size_type m_last_released_data_index;
  size_type m_last_released_data_length;
  size_type m_last_released_space_index;
  size_type m_last_released_space_length;
};

#endif
