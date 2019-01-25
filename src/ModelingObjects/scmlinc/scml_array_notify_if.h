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

#ifndef SCML_ARRAY_NOTIFY_IF_H
#define SCML_ARRAY_NOTIFY_IF_H

#include <scml2_logging/snps_vp_dll.h>

class scml_array_base;

class SNPS_VP_API scml_array_notify_if {
public:
  typedef scml_array_base * array_base_pointer_type;

  virtual void claim_data_notification( array_base_pointer_type );
  virtual void claim_space_notification( array_base_pointer_type );

protected:
  scml_array_notify_if();
  virtual ~scml_array_notify_if();

private: 
  // disable
  scml_array_notify_if( const scml_array_notify_if & );
  scml_array_notify_if & operator= ( const scml_array_notify_if & );
};

#endif
