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
// Modifications : 
//
// Purpose of file : definition of class scml_memory_map_facade
//                   facade between scml_memory and Liberty System
//
*/

#ifndef SCML_MEMORY_MAP_FACADE_H
#define SCML_MEMORY_MAP_FACADE_H

#include <vector>
#include "scmlinc/scml_memory_map_observer_if.h"
#include <scml2_logging/snps_vp_dll.h>
#include <utility>

class SNPS_VP_API scml_memory_map_facade {
public:
  static scml_memory_map_facade & instance();
  void on_memory_map_changed() const;

  void registerObserver( scml_memory_map_observer_if * );
  void unregisterObserver( scml_memory_map_observer_if * );

private:
  scml_memory_map_facade();
  ~scml_memory_map_facade();

private:
  // data members
  typedef ::std::vector< scml_memory_map_observer_if * > vector_observer_type;
  vector_observer_type mObservers;

private:
  // disable
  scml_memory_map_facade( const scml_memory_map_facade & );
  scml_memory_map_facade & operator= ( const scml_memory_map_facade & );
};

#endif
