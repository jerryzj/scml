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
 * Purpose of file : definition of interface for a memory map observer
 *
 */

#ifndef SCML_MEMORY_MAP_OBSERVER_IF_H
#define SCML_MEMORY_MAP_OBSERVER_IF_H

class scml_memory_map_observer_if  {
public:
  virtual void memory_map_changed() {}

protected:
  scml_memory_map_observer_if() {}
  virtual ~scml_memory_map_observer_if() {}

private:
  // disable
  scml_memory_map_observer_if( const scml_memory_map_observer_if & );
  scml_memory_map_observer_if & operator= ( const scml_memory_map_observer_if & );
};

#endif
