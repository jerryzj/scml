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
 * Purpose of file : Definition of scml_bitfield modeling object
 *
 */

#ifndef SCML_BITFIELD_BASE_H
#define SCML_BITFIELD_BASE_H

#include "scmlinc/scml_bitfield_callback_observers.h"
#include <scml2_logging/snps_vp_dll.h>
#include <cstddef>
#include <vector>
#include <string>

/************************************************************************
 * Abstract base class for scml_bitfield;
 */
class scml_bitfield;
class scml_bitfield_callback_observer;

class SNPS_VP_API scml_bitfield_base {
public:
  typedef scml_bitfield_base this_type;
  typedef this_type * pointer_type;
  typedef unsigned int size_type;
  typedef unsigned int data_type; // compatibility with pv32_if
  typedef data_type value_type; // compatibility with pv32_if
  typedef scml_bitfield & scml_bitfield_reference;
  typedef scml_bitfield * scml_bitfield_container_entry;
  typedef ::std::vector< pointer_type> bitfield_pointer_container;

public:
  virtual size_type getBitfieldOffset() const;
  virtual size_type getBitfieldSize() const;
  virtual const char * getBitfieldName() const;
  virtual const char * getName() const;

  virtual bool hasTransportCallBack() const;
  virtual bool hasReadCallBack() const;
  virtual bool hasWriteCallBack() const;

  virtual bool getTransportCallBackName(std::string&) const;
  virtual bool getReadCallBackName(std::string&) const;
  virtual bool getWriteCallBackName(std::string&) const;

  bool registerCallbackObserver(scml_bitfield_callback_observer* o);
  bool unregisterCallbackObserver(scml_bitfield_callback_observer* o);
  bool hasCallbackObservers() const { return mCallbackObservers.has_observers(); }

protected:
  scml_bitfield_base();
public:
  virtual ~scml_bitfield_base();
  
private:
  // disable
  scml_bitfield_base( const scml_bitfield_base & );
  scml_bitfield_base & operator= ( const scml_bitfield_base & );

protected:
  scml_bitfield_callback_observers mCallbackObservers;
};

#endif
